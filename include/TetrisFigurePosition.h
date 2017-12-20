#ifndef _INCLUDE_TETRISFIGUREPOSITION_H_
#define _INCLUDE_TETRISFIGUREPOSITION_H_
#include <utility_func.h>

typedef enum {
    FIG_SQUARE,
    FIG_Z,
    FIG_BACK_Z,
    FIG_R,
    FIG_BACK_R,
    FIG_STICK,
    FIG_T,
	FIG_COUNT
} ENUM_FIGURES;

typedef struct {
    char x;
    char y;
} FLD_POINT;

struct TetrisFigurePosition {
    char size_x;
    char size_y;
    ENUM_FIGURES figure_type;
    char layout[4][4];
    struct TetrisFigurePosition* prev_pos;
    struct TetrisFigurePosition* next_pos;
    char cnt_upper_points;
    char cnt_lower_points;
    FLD_POINT upper_points[4];
    FLD_POINT lower_points[4];
    short rel_start_y;
    int render();
};


const char NUM_FIGURES = 7;

const char NUM_FIG_POS_SQUARE = 1;
const char NUM_FIG_POS_Z = 2;
const char NUM_FIG_POS_BACK_Z = 2;
const char NUM_FIG_POS_R = 4;
const char NUM_FIG_POS_BACK_R = 4;
const char NUM_FIG_POS_STICK = 2;
const char NUM_FIG_POS_T = 4;

#endif
