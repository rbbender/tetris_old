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
	posWndX(0), posWndY(0),
	posScoreX(0), posScoreY(0),
	posLevelX(0), posLevelY(0),
	posNextX(0), posNextY(0),
	szFontHeightPx(0),
	szPrevOffset(0), szCurOffset(0)
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
    setup(0, 0, SZ_BLOCK_PX * (szFldBlkX + 3), SZ_BLOCK_PX, SZ_BLOCK_PX * (szFldBlkX + 3), 3*SZ_BLOCK_PX);
    return 0;
}

XRenderer::~XRenderer() {
    XDestroyWindow(pDsp, wnd);
    XCloseDisplay(pDsp);
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

int XRenderer::render(double ratio) {
	if (ratio >= 1.0)
		ratio = 0.0;
	if (pField->is_redraw_required()) {
		szPrevOffset = 0.0;
		return redraw_full();
	}
	else
		return redraw_delta(ratio);
}

int XRenderer::setup(unsigned short fld_x, unsigned short fld_y, unsigned short score_x, unsigned short score_y,
		unsigned short next_blk_x, unsigned short next_blk_y) {
    for (short i=0; i < NUM_COLORS; ++i) {
    	gcs[i] = XCreateGC(pDsp, wnd, 0, 0);
    	XSetBackground(pDsp, gcs[i], get_color(BLACK));
    	XSetForeground(pDsp, gcs[i], get_color((ENUM_COLORS)i));
    }
    posWndX = fld_x;
    posWndY = fld_y;
    posScoreX = score_x;
    posScoreY = score_y;
    pFontStruct = XQueryFont(pDsp, XGContextFromGC(get_gc_for_color(WHITE)));
    szFontHeightPx = pFontStruct->ascent + pFontStruct->descent;
    posLevelX = posScoreX;
    posLevelY = posScoreY + szFontHeightPx;
    posNextX = next_blk_x;
    posNextY = next_blk_y;
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
    XDrawRectangle(pDsp, wnd, get_gc_for_color(WHITE), posWndX, posWndY, SZ_BLOCK_PX * (szFldBlkX + 2),
        SZ_BLOCK_PX * (szFldBlkY - FLD_VIS_Y + 2));
    XDrawRectangle(pDsp, wnd, get_gc_for_color(WHITE), posWndX + SZ_BLOCK_PX - 1, posWndY + SZ_BLOCK_PX - 1,
        SZ_BLOCK_PX * szFldBlkX + 1, SZ_BLOCK_PX * (szFldBlkY - FLD_VIS_Y) + 1);
    // Flush field
    XFillRectangle(pDsp, wnd, get_gc_for_color(BLACK), posWndX + SZ_BLOCK_PX, posWndY + SZ_BLOCK_PX,
        SZ_BLOCK_PX * szFldBlkX, SZ_BLOCK_PX * (szFldBlkY - FLD_VIS_Y));
    // Flush score
    XFillRectangle(pDsp, wnd, get_gc_for_color(BLACK), posScoreX, posScoreY, szFontHeightPx * 6,
        szFontHeightPx);
    // Flush level
    XFillRectangle(pDsp, wnd, get_gc_for_color(BLACK), posLevelX, posLevelY, szFontHeightPx * 6,
        szFontHeightPx);
    // Flush next figure
    XFillRectangle(pDsp, wnd, get_gc_for_color(BLACK), posNextX, posNextY, SZ_BLOCK_PX * 4,
        SZ_BLOCK_PX * 4);
    XFlush(pDsp);
    return 0;
}

int XRenderer::redraw_full() {
    draw_empty_field();
    int t_fld_x = posWndX + SZ_BLOCK_PX;
    int t_fld_y = posWndY + SZ_BLOCK_PX;
    // Draw field
    for (unsigned y=FLD_VIS_Y; y<szFldBlkY; ++y) {
        for (unsigned x=0; x<szFldBlkX; ++x) {
        	ENUM_COLORS cur_color = (ENUM_COLORS) pField->get_fld_pnt(x, y);
            if (cur_color != BLACK) {
                XFillRectangle(pDsp, wnd, get_gc_for_color(cur_color), t_fld_x + SZ_BLOCK_PX * x,
                    t_fld_y + SZ_BLOCK_PX * (y - FLD_VIS_Y), SZ_BLOCK_PX, SZ_BLOCK_PX);
            }
        }
    }
    figure_position_t* next_position = pField->get_next_figure();
    ENUM_COLORS next_color = pField->get_next_color();
    for (int y=0; y<next_position->size_y; ++y)
        for (int x=0; x<next_position->size_x; ++x)
            if (next_position->layout[y][x] == 1)
                XFillRectangle(pDsp, wnd, get_gc_for_color(next_color), posNextX + SZ_BLOCK_PX * x,
                    posNextY + SZ_BLOCK_PX * y, SZ_BLOCK_PX, SZ_BLOCK_PX);
    std::stringstream oStr;
    oStr<< "Score: " << pField->get_points();
    const char* pOStr = oStr.str().c_str();
    size_t szOStr = oStr.str().length();
    XDrawString(pDsp, wnd, get_gc_for_color(WHITE), posScoreX, posScoreY + pFontStruct->ascent, pOStr, szOStr);
    oStr.str("");
    oStr.clear();
    oStr << "Level: " << pField->get_level();
    pOStr = oStr.str().c_str();
    szOStr = oStr.str().length();
    XDrawString(pDsp, wnd, get_gc_for_color(WHITE), posLevelX, posLevelY + pFontStruct->ascent, pOStr, szOStr);
    XFlush(pDsp);
    pField->unset_redraw_flag();
    pField->clear_new_rectangles();
    pField->clear_deleted_rectangles();
    return 0;
}

int XRenderer::redraw_delta(double ratio) {
	DEBUG_TRACE;
	std::deque<FieldAddr_t>& deleted_rectangles = pField->get_deleted_rectangles();
	std::deque<FieldAddr_t>& new_rectangles = pField->get_new_rectangles();
    DEBUG_VAR("%lu\n", deleted_rectangles.size());
    DEBUG_VAR("%lu\n", new_rectangles.size());
	std::deque<XRectangle> x_deleted_rectangles;
	std::deque<XRectangle> x_new_rectangles;
	szCurOffset = (short) (ratio * SZ_BLOCK_PX);
	if (szCurOffset >= SZ_BLOCK_PX)
		szCurOffset = SZ_BLOCK_PX;
	for (auto i = deleted_rectangles.cbegin(), e = deleted_rectangles.cend();
			i != e; ++i)
	{
		XRectangle tmp = {(short)(posWndX + SZ_BLOCK_PX * (i->second + 1)), (short)(posWndY + SZ_BLOCK_PX * (i->first + 1) + szPrevOffset),
				SZ_BLOCK_PX, SZ_BLOCK_PX};
		x_deleted_rectangles.push_back(tmp);
	}
	for (auto i = new_rectangles.cbegin(), e = new_rectangles.cend();
			i != e; ++i)
	{
		XRectangle tmp = {(short)(posWndX + SZ_BLOCK_PX * (i->second + 1)), (short)(posWndY + SZ_BLOCK_PX * (i->first + 1) + szCurOffset),
				SZ_BLOCK_PX, SZ_BLOCK_PX};
		x_new_rectangles.push_back(tmp);
	}
#ifdef DEBUG
    DEBUG_PRINT("x_redraw_delta: deleted_rectangles:\n");
    for (auto i=x_deleted_rectangles.begin(), e = x_deleted_rectangles.end(); i != e; ++i)
        DEBUG_PRINT("x=%d y=%d\n", i->x, i->y);
    DEBUG_PRINT("x_redraw_delta: new_rectangles:\n");
    for (auto i=x_new_rectangles.begin(), e = x_new_rectangles.end(); i != e; ++i)
        DEBUG_PRINT("x=%d y=%d\n", i->x, i->y);
#endif
    XFillRectangles(pDsp, wnd, get_gc_for_color(BLACK), &(*(x_deleted_rectangles.begin())), x_deleted_rectangles.size());
    XFillRectangles(pDsp, wnd, get_gc_for_color(pField->get_cur_color()), &(*(x_new_rectangles.begin())), x_new_rectangles.size());
    XFlush(pDsp);
    szPrevOffset = szCurOffset;
    new_rectangles.clear();
    deleted_rectangles.clear();
    return 0;
}

