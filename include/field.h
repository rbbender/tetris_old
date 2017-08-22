#include <figure.h>
#include <deque>

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
    int prev_y;
    int prev_x;
    unsigned long long points;
    bool redraw_required = true;
    bool to_exit = false;
    bool is_x = false;

    std::deque<XRectangle> new_rectangles, deleted_rectangles;

    Display* disp;
    Window* wnd;
    GC* gcwhite;
    GC* gcblack;
    short field_x, field_y;
    int scor_x, scor_y;
    int level_x, level_y;
    short next_x, next_y;
    unsigned font_height_px;
    XFontStruct* p_fontstruct;
    unsigned score_sz_px;
    unsigned level_sz_px;
    unsigned level;

public:
    field_t();
    int x_setup(Display*, Window*, GC*, GC*, int, int, int, int, int, int);
    int x_render();
    int x_draw_empty_field();
    int x_set_rectangle_black(short, short);
    int x_set_rectangle_white(short, short);
    int x_redraw_full();
    int x_redraw_delta();
    int remove_previous();
    int render_current();
    int recompose();
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
    void increase_level() {++level;};
};
