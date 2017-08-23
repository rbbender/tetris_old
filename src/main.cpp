#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <field.h>
#include <controls.h>


field_t* game_field;

Display* dpy;
Window w;
GC gcw, gcb;
unsigned whitePixel, blackPixel;

const int TICS_PER_SECOND = 20;
double LEVEL_START_MSEC;
int tic_freq = TICS_PER_SECOND;
double MSEC_PER_TIC = 1.0/TICS_PER_SECOND;


int process_input() {
    static char buf[255];
    XEvent e;
    int c;
    if (XCheckWindowEvent(dpy, w, KeyPressMask, &e) == True && 
        XLookupString(&e.xkey, buf, 255, NULL, NULL) == 1) {
        c = buf[0];
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
    static unsigned int next_level;
    if (next_tic == 0)
        next_tic = TICS_PER_SECOND;
    if (next_level == 0)
        next_level = 5;
    if (game_field->get_points() >= next_level) {
        next_level += 5;
        game_field->increase_level();
        LEVEL_START_MSEC = get_time();
        if (tic_freq > 2)
        	tic_freq -= 2;
        next_tic = tic_freq;
    }
    if (current_tics >= next_tic) {
        DEBUG_VAR("%f\n", LEVEL_START_MSEC);
        next_tic += tic_freq;
        return game_field->tick();
    }
    else {
    	return game_field->inter_tick((double)current_tics/next_tic);
    }
    return 0;
}

int init_x() {
    dpy = XOpenDisplay(0);
    blackPixel = BlackPixel(dpy, DefaultScreen(dpy));
    whitePixel = WhitePixel(dpy, DefaultScreen(dpy));
    w = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 
        0, 400, 600, 0, blackPixel, 
        blackPixel);
    XSetStandardProperties(dpy, w, "Super tetris", 
        "Tetris", None, NULL, 0, NULL);
    XSelectInput(dpy, w, ExposureMask|KeyPressMask|StructureNotifyMask);
    gcw = XCreateGC(dpy, w, 0, 0);
    gcb = XCreateGC(dpy, w, 0, 0);
    XSetForeground(dpy, gcw, whitePixel);
    XSetBackground(dpy, gcw, blackPixel);
    XSetBackground(dpy, gcb, blackPixel);
    XSetForeground(dpy, gcb, blackPixel);
    XClearWindow(dpy, w);
    XMapRaised(dpy, w);
    while (true) {
        XEvent e;
        XNextEvent(dpy, &e);
        if (e.type == MapNotify)
            break;
    }
    return 0;
}

int close_x() {
    XFreeGC(dpy, gcw);
    XFreeGC(dpy, gcb);
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);
    return 0;
}

int GAME_INIT(time_t seed=0) {
    if (seed == 0)
        seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    init_x();
    game_field = new field_t();
    game_field->x_setup(dpy, &w, &gcb, &gcw, 0, 0, 260, 20, 260, 60);
    game_field->x_render();
    LEVEL_START_MSEC = get_time();
    return 0;
}

int MAIN_LOOP() {
    int tick_res = 0;
    double current, diff;
    game_field->x_render();
	while (!tick_res) {
        current = get_time() - LEVEL_START_MSEC;
        process_input();
        tick_res = update_state(current / MSEC_PER_TIC);
        game_field->x_render();
        //game_field->print();
        diff = current + MSEC_PER_TIC - get_time();
        //printf("usleep(%f)\n", diff);
        if (diff > 0)
            usleep(diff);
	}
	return 0;
}

int GAME_END() {
    printf("Game is over, your score is %Lu\n", game_field->get_points());
    close_x();
    return 0;
}

int main() {
    DEBUG_VAR("%f\n", MSEC_PER_TIC);
	GAME_INIT();
	MAIN_LOOP();
    GAME_END();
    return 0;
	//print_field();
	//std::cout << "------\n";
}

