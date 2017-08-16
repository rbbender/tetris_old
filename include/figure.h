#include <figure_position.h>

struct figure_t {
    int pos_x;
    int pos_y;
    ENUM_FIGURES type;
    struct figure_position_t* current_pos;
    figure_t(ENUM_FIGURES type, figure_position_t* pos, int p_y);
    int is_there_collision(figure_position_t* pos);
    int rotate_clockwise();
    int rotate_counterclockwise();
    int render();
};