#include <utility_func.h>
#include <colors.h>

char const* color_names[NUM_COLORS] = {"Black", "Red", "Orange", "Yellow", "Green", "Blue", "White"};
unsigned colors[NUM_COLORS];

unsigned get_color(ENUM_COLORS color) {
    if (color >= 0 && color < NUM_COLORS)
        return colors[color];
    return -1;
}
