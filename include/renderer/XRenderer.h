/*
 * XRenderer.h
 *
 *  Created on: Nov 12, 2017
 *      Author: rbbender
 */

#ifndef INCLUDE_RENDERER_XRENDERER_H_
#define INCLUDE_RENDERER_XRENDERER_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "colors.h"
#include "renderer/Renderer.h"

int colors_fill(Display*, int);

class XRenderer: public Renderer {
	Display* pDsp;
	Window wnd;
    GC gcs[NUM_COLORS];
    XFontStruct* pFontStruct;
    const unsigned short SZ_BLOCK_PX = 20;

    // visual elements addresses
    unsigned short posWndX, posWndY; // window size
    unsigned short posScoreX, posScoreY; // current score position
    unsigned short posLevelX, posLevelY; // current difficulty level position
    unsigned short posNextX, posNextY; // next figure position
    unsigned short szFontHeightPx; // current font height
    unsigned short szPrevOffset, szCurOffset;

    int setup(unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short);
    int draw_empty_field();
    int set_rectangle_black(short, short);
    int set_rectangle_white(short, short);
    int redraw_full();
    int redraw_delta(double ratio);
    int fill_prev_black();
    int fill_cur_white();
    GC get_gc_for_color(ENUM_COLORS);

public:
    XRenderer();
    ~XRenderer();
	int init(field_t* field);
	int process_input();
	int render(double ratio);
};


#endif /* INCLUDE_RENDERER_XRENDERER_H_ */
