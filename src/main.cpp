#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <field.h>
#include <controls.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


field_t* game_field;

Display* dpy;
Window w;
GLFWwindow* wnd;

const int TICS_PER_SECOND = 20;
unsigned LEVEL_START_MSEC;
int tic_freq = TICS_PER_SECOND;
unsigned MSEC_PER_TIC = 1000000/TICS_PER_SECOND;
int prev_tic = 0, next_tic = TICS_PER_SECOND;


int x_process_input() {

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

}

int close_x() {
    XDestroyWindow(dpy, w);
    XCloseDisplay(dpy);
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int init_gl() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	wnd = glfwCreateWindow(640, 480, "Tetris", NULL, NULL);
	if (!wnd){
		printf("Unable to create window!\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(wnd);
	glfwSetFramebufferSizeCallback(wnd, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Unable to initialize GLAD\n");
		return -1;
	}

	return 0;
}

int close_gl() {
	glfwTerminate();
	return 0;
}

int GAME_INIT_X(time_t seed=0) {
    if (seed == 0)
        seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    printf("MSEC_PER_TIC = %u\n", MSEC_PER_TIC);
    init_x();
    game_field = new field_t();
    game_field->x_setup(dpy, &w, 0, 0, 260, 20, 260, 60);
    game_field->x_render();
    return 0;
}

int GAME_INIT_GL(time_t seed=0) {
    if (seed == 0)
        seed = time(NULL);
    srand(seed);
    printf("Initialized game with seed = %ld\n", seed);
    printf("MSEC_PER_TIC = %u\n", MSEC_PER_TIC);
    init_gl();
    game_field = new field_t();
    game_field->gl_setup();
    game_field->gl_render();
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
        x_process_input();
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

int GAME_END_X() {
    printf("Game is over, your score is %Lu\n", game_field->get_points());
    close_x();
    return 0;
}


int GAME_END_GL() {
    printf("Game is over, your score is %Lu\n", game_field->get_points());
    close_gl();
    return 0;
}

int main() {
    DEBUG_VAR("%u\n", MSEC_PER_TIC);
	GAME_INIT_GL();
	MAIN_LOOP();
    GAME_END_GL();
    return 0;
	//print_field();
	//std::cout << "------\n";
}

