#include <figure.h>
#include <deque>
#include <utility>

typedef std::pair<int, int> FieldAddr_t;

class field_t {
    enum fld_sz {
        SZ_X=10,
        SZ_Y=23,
    };

    const short VIS_Y = 3;
    const unsigned short X_BLOCK_SZ = 20;

    char fld[SZ_Y][SZ_X];
    figure_t* current_figure;
    figure_position_t* prev_position;
    figure_position_t* next_position;
    ENUM_COLORS cur_color, next_color;
    int prev_y;
    int prev_x;
    unsigned long long points;
    bool redraw_required = true;
    bool to_exit = false;
    bool is_landed = false;

    std::deque<FieldAddr_t> new_rectangles, deleted_rectangles;

    unsigned prev_offset, cur_offset;
    short field_x, field_y;
    int scor_x, scor_y;
    int level_x, level_y;
    short next_x, next_y;
    unsigned level;

public:
    field_t();
    int remove_previous();
    int render_current();
    int recompose();
    int tick();
    int inter_tick(double tick_ratio);
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
    int get_field_size_x() {return SZ_X};
    int get_field_size_y() {return SZ_Y};
    void print();
    void set_redraw_flag() {redraw_required = true;};
    void set_exit_flag() {to_exit = true;};
    void exit();
    void force_landing();
    void increase_level() {++level;};
    int set_prev_remove();
    int set_cur_new();
};
