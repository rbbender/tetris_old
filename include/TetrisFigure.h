#ifndef _INCLUDE_TETRISFIGURE_H_
#define _INCLUDE_TETRISFIGURE_H_
#include <colors.h>
#include <TetrisFigurePosition.h>

struct TetrisFigure {
    TetrisFigurePosition* current_pos;
    short pos_x;
    short pos_y;
    ENUM_COLORS color;
    ENUM_FIGURES type;
    TetrisFigure(ENUM_FIGURES type, TetrisFigurePosition* pos, ENUM_COLORS col);
    int copy(TetrisFigure* src);
    int is_there_collision(TetrisFigurePosition* pos);
    int rotate_clockwise();
    int rotate_counterclockwise();
    int render();
};
#endif
