#include <TetrisFigure.h>
#include <cstdio>

TetrisFigure::TetrisFigure(ENUM_FIGURES type, TetrisFigurePosition* pos, ENUM_COLORS col):
        current_pos(pos),
        pos_x(4),
        pos_y(pos->rel_start_y),
		color(col),
        type(type)
{
}

TetrisFigure::TetrisFigure():
		current_pos(nullptr),
		pos_x(0),
		pos_y(0),
		color(WHITE),
		type(FIG_SQUARE)
	{
}

int TetrisFigure::rotate_clockwise() {
    current_pos = current_pos->next_pos;
    return 0;
}

int TetrisFigure::rotate_counterclockwise() {
    current_pos = current_pos->prev_pos;
    return 0;
}

int TetrisFigure::render() {
    for (int i=0; i < current_pos->size_y; ++i) {
        for (int k=0; k < current_pos->size_x; ++k)
            printf("%d", current_pos->layout[i][k]);
        printf("\n");
    }
    return 0;
} 

int TetrisFigure::copy(TetrisFigure* src) {
	current_pos = src->current_pos;
	pos_x = src->pos_x;
	pos_y = src->pos_y;
	color = src->color;
	type = src->type;
	return 0;
}

