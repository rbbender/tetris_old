#include <figure_position.h>
#include <colors.h>

struct figure_t {
    short pos_x;
    short pos_y;
    ENUM_COLORS color;
    ENUM_FIGURES type;
    struct figure_position_t* current_pos;
    figure_t(ENUM_FIGURES type, figure_position_t* pos, short p_y);
    int is_there_collision(figure_position_t* pos);
    int rotate_clockwise();
    int rotate_counterclockwise();
    int render();
};
