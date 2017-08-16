#include <figure.h>

class field_t {
    enum fld_sz {
        SZ_X=10,
        SZ_Y=23,
    };

    const char VIS_Y = 3;

    char fld[SZ_Y][SZ_X];
    figure_t* current_figure;
    figure_position_t* prev_position;
    figure_position_t* next_position;
    int prev_y;
    int prev_x;
    unsigned long long points;
    bool redraw_required = false;
    bool to_exit = false;
public:
    field_t();
    int remove_previous();
    int render_current();
    int render();
    int tick();
    int rotate_clockwise();
    int rotate_counterclockwise();
    int move_left();
    int move_right();
    int get_figure_start_position_y(figure_position_t* pos);
    bool is_game_ended();
    int remove_line(int n_line);
    int remove_full_lines();
    unsigned long long get_points();
    figure_position_t* next_figure();
    bool is_figure_landed();
    bool is_rotation_possible(figure_position_t* candidate_pos);
    bool is_move_left_possible();
    bool is_move_right_possible();
    void print();
    void set_redraw_flag() {redraw_required = true;};
    void set_exit_flag() {to_exit = true;};
    void exit();
    void force_landing();
};
