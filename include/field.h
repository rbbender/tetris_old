#include <figure.h>
#include <deque>
#include <utility>

typedef std::pair<unsigned short, unsigned short> FieldAddr_t;

class field_t {
    enum fld_sz {
        SZ_X=10,
        SZ_Y=23,
    };

    const short VIS_Y = 3;

    char fld[SZ_Y][SZ_X];
    figure_t* current_figure;
    figure_position_t* prev_position;
    figure_position_t* next_position;
    int prev_y;
    int prev_x;
    bool redraw_required = true;
    bool to_exit = false;
    bool is_landed = false;

    std::deque<FieldAddr_t> new_rectangles, deleted_rectangles;

public:
    field_t();
    // current figure movement
    //// verifications
    bool is_figure_landed();
    bool is_rotation_possible(figure_position_t* candidate_pos);
    bool is_move_left_possible();
    bool is_move_right_possible();
    //// movements
    int rotate_clockwise();
    int rotate_counterclockwise();
    int move_left();
    int move_right();
    void force_landing();
    // update field layout
    int remove_previous();
    int render_current();
    int recompose();
    int remove_line(int n_line);
    int remove_full_lines();
    // access methods
    int get_figure_start_position_y(figure_position_t* pos);
    int get_field_size_x() {return SZ_X;};
    int get_field_size_y() {return SZ_Y;};
    short get_vis_y() {return VIS_Y;};
    figure_position_t* get_next_figure();
    std::deque<FieldAddr_t>& get_new_rectangles();
    std::deque<FieldAddr_t>& get_deleted_rectangles();
    // updated squares operations (for delta redraw)
    int set_prev_remove();
    int set_cur_new();
    int clear_deleted_rectangles();
    int clear_new_rectangles();
    int get_fld_pnt(int x, int y);
    // debug
    void print();
};
