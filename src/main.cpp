#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <field.h>
#include <controls.h>

#include <utility_func.h>
#include <renderer/XRenderer.h>

#include <assert.h>

field_t* game_field;

Renderer* rnd;

const int TICS_PER_SECOND = 20;
unsigned LEVEL_START_MSEC;
int tic_freq = TICS_PER_SECOND;
unsigned int tics_per_turn = TICS_PER_SEC; // tics per turn (tpt, velocity) - initially equals to our default framerate, will decrease with time
										// means less tics per turn (milliseconds per tic is constant)
										// initially one turn will take 1 second. Less tpt - less time being consumed by turn
unsigned long long prev_turn = 0, next_turn = tics_per_turn;
unsigned int next_level = 5;
int tick_res = 0;


int update_state(unsigned long long current_tics, double tic_ratio) {
    //printf("current_tics=%d\n", current_tics);
    ENUM_TIC_RESULT tic_result;
    int result = 0;
    DEBUG_VAR("%d\n", current_tics);
    DEBUG_VAR("%d\n", next_turn);

	tic_result = game_field->tic(tic_ratio);

	switch (tic_result) {
	case TIC_RESULT_FIGURE_LANDED:
		if (game_field->get_points() >= next_level) {
			next_level += 5;
			game_field->increase_level();
			if (tics_per_turn > 2)
				tics_per_turn -= 2;
		};
		prev_turn = current_tics;
		next_turn = prev_turn + tics_per_turn;
		break;
	case TIC_RESULT_TURN:
		prev_turn = current_tics;
		next_turn = prev_turn + tics_per_turn;
		break;
	case TIC_RESULT_GAME_OVER:
		result = 1;
		break;
	case TIC_RESULT_PLAY_ANIMATION:
		break;
	}
    return result;
}

void game_timer_cb() {
	unsigned long long current_time_ms = get_time_since_start_ms();
	rnd->process_input();
	auto current_tic = get_tic(current_time_ms);
	double tic_ratio = (double) (current_tic - prev_turn) / tics_per_turn;
	DEBUG_VAR("%llu\n", current_tic);
	DEBUG_VAR("%llu\n", prev_turn);
	DEBUG_VAR("%u\n", tics_per_turn);
	DEBUG_VAR("%f\n", tic_ratio);
	if (tic_ratio > 1.0) // to avoid rounding error
		tic_ratio = 1.0;
	//assert(tic_ratio <= 1.0);
	tick_res = update_state(current_tic, tic_ratio);
	rnd->render(tic_ratio);
	//game_field->print();
	//printf("rounded_tic(%u) MSEC_PER_TIC(%u) get_time(%u)\n", rounded_tic, MSEC_PER_TIC, get_time());
	auto diff = get_time_to_next_tic_ms(current_time_ms);
	//printf("rounded_tic(%u) current(%u) usleep(%u)\n", rounded_tic, current, diff);
	DEBUG_VAR("%u\n", diff);
	if (diff > 0)
		usleep(diff);
}


int GAME_INIT_X(time_t seed=0) {
    if (seed == 0)
        seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    printf("MSEC_PER_TIC = %u\n", MSEC_PER_TIC);
    game_field = new field_t();
    rnd = new XRenderer(game_field);
    rnd->init();
    rnd->render(0.0);
    return 0;
}

int MAIN_LOOP() {
    rnd->render(0.0);
    LEVEL_START_MSEC = get_current_time_ms();
    DEBUG_VAR("%u\n", LEVEL_START_MSEC);
	while (!tick_res) {
		game_timer_cb();
	}
	return 0;
}

int GAME_END_X() {
    printf("Game is over, your score is %Lu\n", game_field->get_points());
    delete rnd;
    return 0;
}


int main() {
    DEBUG_VAR("%u\n", MSEC_PER_TIC);
	GAME_INIT_X();
	MAIN_LOOP();
    GAME_END_X();
    return 0;
	//print_field();
	//std::cout << "------\n";
}

