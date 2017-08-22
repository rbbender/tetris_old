#include <cstdio>
#include <figure.h>

figure_t::figure_t(ENUM_FIGURES type, figure_position_t* pos, short p_y):
        pos_x(4),
        pos_y(p_y),
        type(type),
        current_pos(pos){
}

int figure_t::rotate_clockwise() {
    current_pos = current_pos->next_pos;
    return 0;
}

int figure_t::rotate_counterclockwise() {
    current_pos = current_pos->prev_pos;
    return 0;
}

int figure_t::render() {
    for (int i=0; i < current_pos->size_y; ++i) {
        for (int k=0; k < current_pos->size_x; ++k)
            printf("%d", current_pos->layout[i][k]);
        printf("\n");
    }
    return 0;
} 
