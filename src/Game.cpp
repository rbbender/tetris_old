/*
 * game.cpp
 *
 *  Created on: Dec 15, 2017
 *      Author: vgratsil
 */

#include <Game.h>

extern const char FIG_POS_COUNTS[];
extern TetrisFigurePosition* FIG_POSITIONS[];

Game::Game(Renderer& render):
	TICS_PER_SEC(25),
	MSEC_PER_TIC(1000 / TICS_PER_SEC),
	SCORE_PER_LVL(5),
	gameField(),
	rRender(render),
	game_start_time_ms(get_current_time_ms()),
	prev_turn_time_ms(0),
	tics_per_turn(TICS_PER_SEC),
	next_lvl(SCORE_PER_LVL),
	tic_res(0),
	score(0),
	next_color(WHITE)
{
	time_t seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    printf("MSEC_PER_TIC = %u\n", MSEC_PER_TIC);
    render.init(get_field());
}

int Game::update_state(unsigned long long current_tics, double tic_ratio) {
    //printf("current_tics=%d\n", current_tics);
    ENUM_TIC_RESULT tic_result;
    int result = 0;
    DEBUG_VAR("%llu\n", current_tics);

	tic_result = gameField.tic(tic_ratio);

	switch (tic_result) {
	case TIC_RESULT_FIGURE_LANDED:
		if (gameField.get_points() >= next_lvl) {
			next_lvl += SCORE_PER_LVL;
			gameField.increase_level();
			if (tics_per_turn > 2)
				tics_per_turn -= 2;
		};
		prev_turn_time_ms = current_tics;
		break;
	case TIC_RESULT_TURN:
		prev_turn_time_ms = current_tics;
		break;
	case TIC_RESULT_GAME_OVER:
		result = 1;
		break;
	case TIC_RESULT_PLAY_ANIMATION:
		break;
	}
    return result;
}

void Game::game_timer_cb() {
	unsigned long long current_time_ms = get_time_since_start_ms();
	rRender.process_input();
	auto current_tic = get_tic(current_time_ms);
	double tic_ratio = (double) (current_tic - prev_turn_time_ms) / tics_per_turn;
	DEBUG_VAR("%llu\n", current_tic);
	DEBUG_VAR("%llu\n", prev_turn_time_ms);
	DEBUG_VAR("%llu\n", tics_per_turn);
	DEBUG_VAR("%f\n", tic_ratio);
	if (tic_ratio > 1.0) // to avoid rounding error
		tic_ratio = 1.0;
	//assert(tic_ratio <= 1.0);
	tic_res = update_state(current_tic, tic_ratio);
	rRender.render(tic_ratio);
	//game_field->print();
	//printf("rounded_tic(%u) MSEC_PER_TIC(%u) get_time(%u)\n", rounded_tic, MSEC_PER_TIC, get_time());
	auto diff = get_time_to_next_tic_ms(current_time_ms);
	//printf("rounded_tic(%u) current(%u) usleep(%u)\n", rounded_tic, current, diff);
	DEBUG_VAR("%llu\n", diff);
	if (!tic_res)
		Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &Game::game_timer_cb), diff);
};

TetrisField* Game::get_field() {
	return &gameField;
}

unsigned long long Game::get_time_since_start_ms() {
	return get_current_time_ms() - game_start_time_ms;
}

unsigned long long Game::get_tic(){
	return get_time_since_start_ms() / MSEC_PER_TIC;
}

unsigned long long Game::get_tic(unsigned long long time_since_start_ms){
	return time_since_start_ms / MSEC_PER_TIC;
}

unsigned long long Game::get_time_to_next_tic_ms(){
	return (get_tic() + 1) * MSEC_PER_TIC - get_time_since_start_ms();
}


unsigned long long Game::get_time_to_next_tic_ms(unsigned long long time_since_start_ms){
	return (get_tic() + 1) * MSEC_PER_TIC - time_since_start_ms;
}

TetrisFigurePosition* Game::next_figure() {
    DEBUG_VAR("%d\n", NUM_FIGURES);
    int nxt_col = rand() % (NUM_COLORS - 1);
    next_color = static_cast<ENUM_COLORS> (nxt_col + 1);
    ENUM_FIGURES n = static_cast<ENUM_FIGURES> (rand() % static_cast<int>(FIG_COUNT));
    int pos = rand() % FIG_POS_COUNTS[n];
    return &FIG_POSITIONS[n][pos];
};
