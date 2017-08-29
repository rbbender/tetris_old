#include <X11/Xlib.h>
#include <X11/Xutil.h>

const char NUM_COLORS=7;

typedef enum {
    BLACK,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    WHITE
} ENUM_COLORS;

extern char const* color_names[NUM_COLORS];
extern int colors[NUM_COLORS];

int colors_fill(Display*, int);
int get_color(int);