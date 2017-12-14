#ifndef INCLUDE_COLORS_H
#define INCLUDE_COLORS_H


typedef enum {
    BLACK,
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    WHITE,
	NUM_COLORS
} ENUM_COLORS;

extern char const* color_names[NUM_COLORS];
extern unsigned colors[NUM_COLORS];

unsigned get_color(ENUM_COLORS);

#endif
