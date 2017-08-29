#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <field.h>
#include <controls.h>
#include <colors.h>


field_t* game_field;

Display* dpy;
Window w;
GC gcw, gcb;
unsigned whitePixel, blackPixel;

const int TICS_PER_SECOND = 20;
unsigned LEVEL_START_MSEC;
int tic_freq = TICS_PER_SECOND;
unsigned MSEC_PER_TIC = 1000000/TICS_PER_SECOND;
int prev_tic = 0, next_tic = TICS_PER_SECOND;


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
    static unsigned int next_level;
    int inter_tic_res;
    if (next_level == 0)
        next_level = 5;
    DEBUG_VAR("%d\n", current_tics);
    DEBUG_VAR("%d\n", next_tic);
    if (game_field->get_points() >= next_level) {
        next_level += 5;
        game_field->increase_level();
        LEVEL_START_MSEC = get_time();
        if (tic_freq > 2)
        	tic_freq -= 2;
        prev_tic = 0;
        next_tic = tic_freq;
        return 0;
    }
    if (current_tics >= next_tic) {
        DEBUG_VAR("%u\n", LEVEL_START_MSEC);
        prev_tic = next_tic;
        next_tic += tic_freq;
        //return 2;
        return game_field->tick();
    }
    else {
        DEBUG_VAR("%d\n", current_tics);
        DEBUG_VAR("%d\n", next_tic);
        inter_tic_res = game_field->inter_tick((double)(current_tics-prev_tic)/tic_freq);
        if (inter_tic_res == 1) {
            prev_tic = current_tics;
            next_tic = current_tics + 1;
        }

    }
    return 0;
}

int init_x() {
    dpy = XOpenDisplay(0);
    blackPixel = BlackPixel(dpy, DefaultScreen(dpy));
    whitePixel = WhitePixel(dpy, DefaultScreen(dpy));
    if (colors_fill(dpy, DefaultScreen(dpy)) < 0) {
        printf("ERROR: unable to initialize colors, exiting...");
        exit(1);
    }
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
    printf("MSEC_PER_TIC = %u\n", MSEC_PER_TIC);
    init_x();
    game_field = new field_t();
    game_field->x_setup(dpy, &w, &gcb, &gcw, 0, 0, 260, 20, 260, 60);
    game_field->x_render();
    return 0;
}

int MAIN_LOOP() {
    int tick_res = 0;
    unsigned rounded_tic = 0;
    unsigned current, diff;
    game_field->x_render();
    LEVEL_START_MSEC = get_time();
    DEBUG_VAR("%u\n", LEVEL_START_MSEC);
	while (!tick_res) {
        current = get_time() - LEVEL_START_MSEC;
        DEBUG_VAR("%u\n", current);
        process_input();
        rounded_tic = current/MSEC_PER_TIC;
        tick_res = update_state(rounded_tic);
        game_field->x_render();
        //game_field->print();
        //printf("rounded_tic(%u) MSEC_PER_TIC(%u) get_time(%u)\n", rounded_tic, MSEC_PER_TIC, get_time());
        diff = MSEC_PER_TIC * (1 + rounded_tic) - current;
        //printf("rounded_tic(%u) current(%u) usleep(%u)\n", rounded_tic, current, diff);
        DEBUG_VAR("%u\n", diff);
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
    DEBUG_VAR("%u\n", MSEC_PER_TIC);
	GAME_INIT();
	MAIN_LOOP();
    GAME_END();
    return 0;
	//print_field();
	//std::cout << "------\n";
}

