#include <TetrisFigure.h>
#include <deque>
#include <utility>
#include <memory>

typedef std::pair<unsigned short, unsigned short> FieldAddr_t;

class TetrisField {
    enum fld_sz {
        SZ_X=10,
        SZ_Y=23,
    };

    const short VIS_Y = 3;

    char fld[SZ_Y][SZ_X];
    std::unique_ptr<TetrisFigure> current_figure;
    std::unique_ptr<TetrisFigure> prev_figure;

    std::deque<FieldAddr_t> new_rectangles, deleted_rectangles;

public:
    TetrisField();
    // current figure movement
    //// verifications
    bool is_figure_landed();
    bool is_rotation_possible(TetrisFigurePosition* candidate_pos);
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
    void set_current_figure(std::unique_ptr<TetrisFigure> pos);
    // access methods
    int get_field_size_x() {return SZ_X;};
    int get_field_size_y() {return SZ_Y;};
    short get_vis_y() {return VIS_Y;};
    std::unique_ptr<TetrisFigure> get_current_figure();
    TetrisFigurePosition* get_next_figure();
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
