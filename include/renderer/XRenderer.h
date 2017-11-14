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

class XRenderer: public Renderer {
	Display* pDsp;
	Window wnd;
    GC gcs[NUM_COLORS];
    XFontStruct* pFontStruct;
    const size_t SZ_BLOCK_PX = 20;

    // visual elements addresses
    size_t szWndX, szWndY; // window size
    size_t posScoreX, posScoreY; // current score position
    size_t posLevelX, posLevelY; // current difficulty level position
    size_t szFontHeightPx; // current font height


    int setup(int, int, int, int, int, int);
    int draw_empty_field();
    int set_rectangle_black(short, short);
    int set_rectangle_white(short, short);
    int redraw_full();
    int redraw_delta();
    int fill_prev_black();
    int fill_cur_white();
    GC get_gc_for_color(ENUM_COLORS);

public:
    XRenderer(field_t* pFld);
	int init();
	int process_input();
	int render();
};


#endif /* INCLUDE_RENDERER_XRENDERER_H_ */
