/*
 * game.h
 *
 *  Created on: Dec 15, 2017
 *      Author: vgratsil
 */

#ifndef INCLUDE_GAME_H_
#define INCLUDE_GAME_H_

#include <utility_func.h>
#include <renderer/Renderer.h>
#include <colors.h>
#include <glibmm/main.h>
#include <TetrisField.h>

typedef enum {
	TIC_RESULT_PLAY_ANIMATION,
	TIC_RESULT_TURN,
	TIC_RESULT_FIGURE_LANDED,
	TIC_RESULT_GAME_OVER
} ENUM_TIC_RESULT;

class Game {
public:
	Game(Renderer& render);
	void game_timer_cb();
	TetrisField* get_field();
    ENUM_TIC_RESULT tic(double tick_ratio);
    unsigned get_score();
    bool is_game_ended();
    bool is_redraw_required();
    void set_redraw_flag();
    void unset_redraw_flag();
    ENUM_COLORS get_cur_color();
    ENUM_COLORS get_next_color();
    void increase_level();
    unsigned get_level();
    void set_exit_flag();
    void exit();
    int get_figure_start_position_y(TetrisFigurePosition* pos);

    int process_move_left();
    int process_move_right();
    int process_rotate_clockwise();
    int process_rotate_counterclockwise();
    int process_force_landing();
    int process_quit();

	const int TICS_PER_SEC;
	const int MSEC_PER_TIC;
	const int SCORE_PER_LVL;

protected:
	int update_state(unsigned long long current_tics, double tic_ratio);

	unsigned long long get_time_since_start_ms();
	unsigned long long get_tic();
	unsigned long long get_tic(unsigned long long time_since_start_ms);
	unsigned long long get_time_to_next_tic_ms();
	unsigned long long get_time_to_next_tic_ms(unsigned long long time_since_start_ms);
    std::unique_ptr<TetrisFigure> next_figure();

	TetrisField gameField;
	Renderer& rRender;
	unsigned long long game_start_time_ms;
	unsigned long long prev_turn_time_ms;
	unsigned long long tics_per_turn;// tics per turn (tpt, velocity) - initially equals to our default framerate, will decrease with time
										// means less tics per turn (milliseconds per tic is constant)
										// initially one turn will take 1 second. Less tpt - less time being consumed by turn

	unsigned next_lvl;
	unsigned current_lvl;
	int tic_res;
	unsigned score;
    ENUM_COLORS next_color;
    bool to_exit;
    std::unique_ptr<TetrisFigure> p_next_figure;
};




#endif /* INCLUDE_GAME_H_ */
