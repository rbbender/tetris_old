/*
 * XRenderer.cpp
 *
 *  Created on: Nov 12, 2017
 *      Author: rbbender
 */

#include <renderer/XRenderer.h>
#include <field.h>

XRenderer::XRenderer(field_t* pFld):
	Renderer(pFld),
	pDsp(nullptr),
	wnd(0),
	pFontStruct(nullptr),
	szWndX(0), szWndY(0),
	posScoreX(0), posScoreY(0),
	posLevelX(0), posLevelY(0),
	szFldBlkX(0), szFldBlkY(0),
	szFontHeightPx(0)
{}

int XRenderer::init() {
	pDsp = XOpenDisplay(NULL);
    if (colors_fill(pDsp, DefaultScreen(pDsp)) < 0) {
        printf("ERROR: unable to initialize colors, exiting...");
        exit(1);
    }
    wnd = XCreateSimpleWindow(pDsp, DefaultRootWindow(pDsp), 0,
        0, 400, 600, 0, get_color(BLACK),
        get_color(BLACK));
    XSetStandardProperties(pDsp, wnd, "Tetris", "Tetris", None, NULL, 0, NULL);
    XSelectInput(pDsp, wnd, ExposureMask|KeyPressMask|StructureNotifyMask);
    XClearWindow(pDsp, wnd);
    XMapRaised(pDsp, wnd);
    while (true) {
        XEvent e;
        XNextEvent(pDsp, &e);
        if (e.type == MapNotify)
            break;
    }
    return 0;
}


int XRenderer::process_input() {
    static char buf[255];
    XEvent e;
    int c;
    if (XCheckWindowEvent(pDsp, wnd, KeyPressMask, &e) == True &&
        XLookupString(&e.xkey, buf, 255, NULL, NULL) == 1) {
        c = buf[0];
        switch (c) {
            case KEY_FLD_LEFT:
                pField->move_left();
                break;
            case KEY_FLD_RIGHT:
                pField->move_right();
                break;
            case KEY_FLD_ROTATE:
                pField->rotate_clockwise();
                break;
            case KEY_FLD_ROTATE_COUNTER:
                pField->rotate_counterclockwise();
                break;
            case KEY_FLD_EXIT:
                pField->exit();
                break;
            case KEY_FLD_FORCE:
                pField->force_landing();
                break;
        }
    }
    return 0;
}

int XRenderer::setup(int fld_x, int fld_y, int score_x, int score_y, int next_blk_x, int next_blk_y) {
    for (short i=0; i < NUM_COLORS; ++i) {
    	gcs[i] = XCreateGC(pDsp, wnd, 0, 0);
    	XSetBackground(pDsp, gcs[i], get_color(BLACK));
    	XSetForeground(pDsp, gcs[i], get_color((ENUM_COLORS)i));
    }
    szWndX = fld_x;
    szWndY = fld_y;
    posScoreX = score_x;
    posScoreY = score_y;
    pFontStruct = XQueryFont(pDsp, XGContextFromGC(get_gc_for_color(WHITE)));
    szFontHeightPx = pFontStruct->ascent + pFontStruct->descent;
    posLevelX = posScoreX;
    posLevelY = posScoreY + szFontHeightPx;
    return 0;
}

GC XRenderer::get_gc_for_color(ENUM_COLORS color) {
	if (color >= 0 && color < NUM_COLORS) {
		return gcs[color];
	}
	return gcs[WHITE];
}

int XRenderer::draw_empty_field() {
    // Border
    XDrawRectangle(pDsp, wnd, get_gc_for_color(WHITE), szWndX, szWndY, SZ_BLOCK_PX * (SZ_X + 2),
        X_BLOCK_SZ * (SZ_Y - VIS_Y + 2));
    XDrawRectangle(disp, *wnd, x_get_gc_for_color(WHITE), field_x + X_BLOCK_SZ - 1, field_y + X_BLOCK_SZ - 1,
        X_BLOCK_SZ * SZ_X + 1, X_BLOCK_SZ * (SZ_Y - VIS_Y) + 1);
    // Flush field
    XFillRectangle(disp, *wnd, x_get_gc_for_color(BLACK), field_x + X_BLOCK_SZ, field_y + X_BLOCK_SZ,
        X_BLOCK_SZ * SZ_X, X_BLOCK_SZ * (SZ_Y - VIS_Y));
    // Flush score
    XFillRectangle(disp, *wnd, x_get_gc_for_color(BLACK), scor_x, scor_y, score_sz_px,
        font_height_px);
    // Flush level
    XFillRectangle(disp, *wnd, x_get_gc_for_color(BLACK), level_x, level_y, level_sz_px,
        font_height_px);
    // Flush next figure
    XFillRectangle(disp, *wnd, x_get_gc_for_color(BLACK), next_x, next_y, X_BLOCK_SZ * 4,
        X_BLOCK_SZ * 4);
    XFlush(disp);
    return 0;
}

int field_t::x_redraw_full() {
    if (!is_x)
        return 1;
    x_draw_empty_field();
    int t_fld_x = field_x + X_BLOCK_SZ;
    int t_fld_y = field_y + X_BLOCK_SZ;
    // Draw field
    for (int y=VIS_Y; y<SZ_Y; ++y) {
        for (int x=0; x<SZ_X; ++x) {
            if (fld[y][x] != BLACK) {
                XFillRectangle(disp, *wnd, x_get_gc_for_color((ENUM_COLORS)fld[y][x]), t_fld_x + X_BLOCK_SZ * x,
                    t_fld_y + X_BLOCK_SZ * (y - VIS_Y), X_BLOCK_SZ, X_BLOCK_SZ);
            }
        }
    }
    for (int y=0; y<next_position->size_y; ++y)
        for (int x=0; x<next_position->size_x; ++x)
            if (next_position->layout[y][x] == 1)
                XFillRectangle(disp, *wnd, x_get_gc_for_color(next_color), next_x + X_BLOCK_SZ * x,
                    next_y + X_BLOCK_SZ * y, X_BLOCK_SZ, X_BLOCK_SZ);
    char s_buf[255];
    sprintf(s_buf, "Score: %Lu", get_points());
    score_sz_px = XTextWidth(p_fontstruct, s_buf, strlen(s_buf));
    XDrawString(disp, *wnd, x_get_gc_for_color(WHITE), scor_x, scor_y + p_fontstruct->ascent, s_buf, strlen(s_buf));
    sprintf(s_buf, "Level: %u", level);
    level_sz_px = XTextWidth(p_fontstruct, s_buf, strlen(s_buf));
    XDrawString(disp, *wnd, x_get_gc_for_color(WHITE), level_x, level_y + p_fontstruct->ascent, s_buf, strlen(s_buf));
    XFlush(disp);
    redraw_required = false;
    new_rectangles.clear();
    deleted_rectangles.clear();
    return 0;
}

int field_t::x_redraw_delta() {
	DEBUG_TRACE;
    if (!is_x)
        return 1;
    DEBUG_VAR("%lu\n", deleted_rectangles.size());
    DEBUG_VAR("%lu\n", new_rectangles.size());
#ifdef DEBUG
    DEBUG_PRINT("x_redraw_delta: deleted_rectangles:\n");
    for (std::deque<XRectangle>::iterator i=deleted_rectangles.begin(); i != deleted_rectangles.end(); ++i)
        DEBUG_PRINT("x=%d y=%d\n", i->x, i->y);
    DEBUG_PRINT("x_redraw_delta: new_rectangles:\n");
    for (std::deque<XRectangle>::iterator i=new_rectangles.begin(); i != new_rectangles.end(); ++i)
        DEBUG_PRINT("x=%d y=%d\n", i->x, i->y);
#endif
    XFillRectangles(disp, *wnd, x_get_gc_for_color(BLACK), &(*(deleted_rectangles.begin())), deleted_rectangles.size());
    XFillRectangles(disp, *wnd, x_get_gc_for_color(cur_color), &(*(new_rectangles.begin())), new_rectangles.size());
    XFlush(disp);
    prev_offset = cur_offset;
    new_rectangles.clear();
    deleted_rectangles.clear();
    return 0;
}

