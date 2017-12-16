#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <field.h>
#include <controls.h>

#include <utility_func.h>
#include <renderer/XRenderer.h>

#include <glibmm/main.h>

field_t* game_field;

Renderer* rnd;

unsigned int tics_per_turn = TICS_PER_SEC; // tics per turn (tpt, velocity) - initially equals to our default framerate, will decrease with time
										// means less tics per turn (milliseconds per tic is constant)
										// initially one turn will take 1 second. Less tpt - less time being consumed by turn
unsigned long long prev_turn = 0;
unsigned int next_level = 5;
int tick_res = 0;

Glib::RefPtr<Glib::MainLoop> glib_MainLoop;





int GAME_INIT_X(time_t seed=0) {
    rnd = new XRenderer();
    rnd->init();
	rnd->render(0.0);
	glib_MainLoop = Glib::MainLoop::create();
    return 0;
}

int MAIN_LOOP() {
	rnd->render(0.0);
	Glib::signal_timeout().connect_once(sigc::ptr_fun(game_timer_cb), get_time_to_next_tic_ms());
	glib_MainLoop->run();
	//while (!tick_res) {
	//	game_timer_cb();
	//}
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

