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
	current_lvl(1),
	tic_res(0),
	score(0),
	next_color(WHITE),
	to_exit(false),
	to_redraw(true)
{
	time_t seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    printf("MSEC_PER_TIC = %u\n", MSEC_PER_TIC);
    render.init(this);

    auto cur_figure = next_figure();
    gameField.set_current_figure(cur_figure);
    p_next_figure = next_figure();
}

int Game::update_state(unsigned long long current_tics, double tic_ratio) {
    //printf("current_tics=%d\n", current_tics);
    ENUM_TIC_RESULT tic_result;
    int result = 0;
    DEBUG_VAR("%llu\n", current_tics);

	tic_result = tic(tic_ratio);

	switch (tic_result) {
	case TIC_RESULT_FIGURE_LANDED:
		if (get_score() >= next_lvl) {
			next_lvl += SCORE_PER_LVL;
			increase_level();
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
	if (tic_res)
		rRender.wrap_up();
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

bool Game::is_game_ended() {
	return gameField.is_game_ended();
}

bool Game::is_redraw_required() {
	return to_redraw;
}

void Game::set_redraw_flag() {
	to_redraw = true;
}

void Game::unset_redraw_flag() {
	to_redraw = false;
}

ENUM_COLORS Game::get_cur_color() {
	return gameField.get_current_color();
}

ENUM_COLORS Game::get_next_color() {
	return (*p_next_figure).color;
}

unsigned Game::get_level() {
	return current_lvl;
}

void Game::set_exit_flag() {
	to_exit = true;
}

void Game::exit() {
}

int Game::process_move_left() {
	return gameField.move_left();
}

int Game::process_move_right() {
	return gameField.move_right();
}

int Game::process_rotate_clockwise() {
	return gameField.rotate_clockwise();
}

int Game::process_rotate_counterclockwise() {
	return gameField.rotate_counterclockwise();
}

int Game::process_force_landing() {
	return gameField.force_landing();
}

int Game::process_quit() {
	set_exit_flag();
	return 0;
}

const TetrisFigurePosition* Game::get_next_position() {
	return (*p_next_figure).current_pos;
}

std::unique_ptr<TetrisFigure> Game::next_figure() {
    ENUM_COLORS next_color = static_cast<ENUM_COLORS>(rand() % (NUM_COLORS - 1) + 1);
    ENUM_FIGURES n = static_cast<ENUM_FIGURES> (rand() % static_cast<int>(FIG_COUNT));
    int pos = rand() % FIG_POS_COUNTS[n];
    return std::make_unique<TetrisFigure>(n, &FIG_POSITIONS[n][pos], next_color);
};

unsigned Game::get_score() {
	return score;
}

void Game::increase_level() {
	++current_lvl;
	next_lvl += SCORE_PER_LVL;
}

ENUM_TIC_RESULT Game::tic(double tic_ratio) {
    DEBUG_TRACE;
    ENUM_TIC_RESULT result = TIC_RESULT_PLAY_ANIMATION;
    if (to_exit)
        return TIC_RESULT_GAME_OVER;
    if (gameField.is_figure_landed()) {
        gameField.recompose();
        score += gameField.remove_full_lines();
        {
        	auto cur_fig = gameField.get_current_figure();
        }
        if (is_game_ended())
            return TIC_RESULT_GAME_OVER;
        gameField.set_current_figure(p_next_figure);
        p_next_figure = next_figure();
        gameField.recompose();
        set_redraw_flag();
#ifdef DEBUG
        DEBUG_VAR("%lu\n", gameField.get_new_rectangles().size());
        DEBUG_VAR("%lu\n", gameField.get_deleted_rectangles().size());
        DEBUG_VAR("%d\n", gameField.get_prev_x());
        DEBUG_VAR("%d\n", gameField.get_prev_y());
        DEBUG_VAR("%d\n", gameField.get_cur_x());
        DEBUG_VAR("%d\n", gameField.get_cur_y());
#endif
        result = TIC_RESULT_FIGURE_LANDED;
    }
    else {
		if (tic_ratio >= 1.0) {
			gameField.turn();
			result = TIC_RESULT_TURN;
		}
		else {
			result = TIC_RESULT_PLAY_ANIMATION;
		}
    }
#ifdef DEBUG
	//print();
#endif
    DEBUG_TRACE;
    return result;
}

