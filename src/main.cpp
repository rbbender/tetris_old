#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <field.h>
#include <controls.h>



int TICS_PER_SECOND = 20;
double MSEC_PER_TIC = 1.0 / TICS_PER_SECOND;
int tic_freq = TICS_PER_SECOND;

double LEVEL_START_MSEC;

struct termios old_settings, new_settings;

field_t* game_field;

int process_input() {
    int c;
    if (kbhit()) {
        c = getchar();
        switch (c) {
            case KEY_LEFT:
                game_field->move_left();
                break;
            case KEY_RIGHT:
                game_field->move_right();
                break;
            case KEY_ROTATE:
                game_field->rotate_clockwise();
                break;
            case KEY_ROTATE_COUNTER:
                game_field->rotate_counterclockwise();
                break;
            case KEY_EXIT:
                game_field->exit();
                break;
            case KEY_FORCE:
                game_field->force_landing();
                break;
        }
    }
    return 0;
}

int update_state(int current_tics) {
    //printf("current_tics=%d\n", current_tics);
    static int next_tic;
    static int next_level;
    if (next_tic == 0)
        next_tic = TICS_PER_SECOND;
    if (next_level == 0)
        next_level = 5;
    if (game_field->get_points() >= next_level) {
        next_level += 5;
        TICS_PER_SECOND *= 1.33;
        MSEC_PER_TIC = 1.0 / TICS_PER_SECOND;
        LEVEL_START_MSEC = get_time();
        next_tic = tic_freq;
    }
    if (current_tics >= next_tic) {
        printf("LEVEL_START_MSEC = %f\n", LEVEL_START_MSEC);
        next_tic += tic_freq;
        return game_field->tick();
    }
    return 0;
}


int GAME_INIT(time_t seed=0) {
    if (seed == 0)
        seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    int res = tcgetattr(0, &old_settings);
    res = tcgetattr(0, &new_settings);
    new_settings.c_lflag &= ~(ICANON | ECHO);
    res = tcsetattr(0, TCSANOW, &new_settings);
    game_field = new field_t();
    LEVEL_START_MSEC = get_time();
}

int MAIN_LOOP() {
    int tick_res = 0;
    double prevtime = 0;
    double current, diff;
    int tics;
    game_field->print();
	while (!tick_res) {
        current = get_time() - LEVEL_START_MSEC;
        prevtime = current;
        process_input();
        tick_res = update_state(current / MSEC_PER_TIC);
        game_field->print();
        diff = current + MSEC_PER_TIC - get_time();
        //printf("usleep(%f)\n", diff);
        if (diff > 0)
            usleep(diff);
	}
}

int GAME_END() {
    printf("Game is over, your score is %Lu\n", game_field->get_points());
    int res = tcsetattr(0, TCSANOW, &old_settings);
}

int main() {
    printf("MSEC_PER_TIC=%f\n", MSEC_PER_TIC);
	GAME_INIT();
	MAIN_LOOP();
    GAME_END();
	//print_field();
	//std::cout << "------\n";
}

