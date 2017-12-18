#include <colors.h>
#include <TetrisFigurePosition.h>

struct TetrisFigure {
    short pos_x;
    short pos_y;
    ENUM_COLORS color;
    ENUM_FIGURES type;
    struct TetrisFigurePosition* current_pos;
    TetrisFigure(ENUM_FIGURES type, TetrisFigurePosition* pos, short p_y);
    int is_there_collision(TetrisFigurePosition* pos);
    int rotate_clockwise();
    int rotate_counterclockwise();
    int render();
};
