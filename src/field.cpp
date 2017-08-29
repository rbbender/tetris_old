#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <field.h>

extern const char FIG_POS_COUNTS[];
extern figure_position_t* FIG_POSITIONS[]; 

unsigned long long field_t::get_points() {
    return points;
}

field_t::field_t() {
    DEBUG_TRACE;
    DEBUG_VAR("%d", SZ_Y);
    printf("%d\n", SZ_Y);
    for (int i = 0; i < SZ_Y; ++i) 
            for (int k = 0; k < SZ_X; ++k) 
                fld[i][k] = '0';

    figure_position_t* n = next_figure();
    prev_y = get_figure_start_position_y(n);
    current_figure = new figure_t(n->figure_type, n, prev_y);
    prev_x = 4;
    prev_position = current_figure->current_pos;
    next_position = next_figure();
    recompose();
	set_redraw_flag();
	level = 1;
    cur_offset = 0;
    DEBUG_TRACE;
}

int field_t::remove_previous() {
    DEBUG_TRACE;
    for (int i = 0; i < prev_position->size_y; ++i)
        for (int k = 0; k < prev_position->size_x; ++k){
            if (prev_position->layout[i][k] == 1) {
                fld[prev_y + i][prev_x + k] = '0';
            }
        }
    DEBUG_TRACE;
    return 0;
}

int field_t::get_figure_start_position_y(figure_position_t* pos) {
    return VIS_Y - pos->size_y + 1;
}

void field_t::exit() {
    set_exit_flag();
}

void field_t::force_landing() {
    remove_previous();
    while(!is_figure_landed())
        ++(current_figure->pos_y);
    recompose();
}

int field_t::recompose() {
    DEBUG_TRACE;
    if (!is_figure_landed()) {
        x_fill_prev_black();
        x_fill_cur_white();
    }
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (current_figure->current_pos->layout[i][k] == 1) {
                fld[current_figure->pos_y + i][current_figure->pos_x + k] = '1';
            }
    prev_y = current_figure->pos_y;
    prev_x = current_figure->pos_x;
    prev_position = current_figure->current_pos;
    DEBUG_TRACE;
    return 0;
}


int field_t::tick() {
    DEBUG_TRACE;
    if (to_exit)
        return 2;
    cur_offset = 0;
    if (is_figure_landed()) {
        recompose();
        points += remove_full_lines();
        delete current_figure;
        if (is_game_ended())
            return 1;
        prev_y = get_figure_start_position_y(prev_position);
        current_figure = new figure_t(next_position->figure_type, next_position, prev_y);
        prev_position = next_position;
        prev_x = 4;
        next_position = next_figure();
        recompose();
        set_redraw_flag();
        is_landed = false;
//        for (int i=0; i < current_figure->current_pos->size_y; ++i)
//        	for (int k=0; k < current_figure->current_pos->size_x; ++k)
//        		if (current_figure->pos_y + i >= VIS_Y && current_figure->current_pos->layout[i][k])
//        			x_set_rectangle_white(current_figure->pos_x + k, current_figure->pos_y + i);
#ifdef DEBUG
        DEBUG_VAR("%lu\n", new_rectangles.size());
        DEBUG_VAR("%lu\n", deleted_rectangles.size());
        DEBUG_VAR("%d\n", prev_x);
        DEBUG_VAR("%d\n", prev_y);
        DEBUG_VAR("%d\n", current_figure->pos_x);
        DEBUG_VAR("%d\n", current_figure->pos_y);
#endif
        //return 2;
    }
    else {
		remove_previous();
        current_figure->pos_y += 1;
        recompose();
        }
#ifdef DEBUG
	print();
#endif
    DEBUG_TRACE;
    return 0;
}

int field_t::inter_tick(double tick_ratio) {
	if (is_figure_landed())
		return 1;
    // calculate figure animation offset
    DEBUG_VAR("%f\n", tick_ratio);
	assert(tick_ratio >= 0.0);
    prev_offset = cur_offset;
    cur_offset = tick_ratio * X_BLOCK_SZ;
    DEBUG_VAR("%d\n", prev_offset);
    DEBUG_VAR("%d\n", cur_offset);
    if (cur_offset > X_BLOCK_SZ) // to avoid rounding error 
        cur_offset = X_BLOCK_SZ;
    x_fill_prev_black();
    x_fill_cur_white();
    return 0;
}

int field_t::rotate_clockwise() {
    remove_previous();
    if (is_rotation_possible(current_figure->current_pos->next_pos))
        current_figure->rotate_clockwise();
    recompose();
    new_rectangles.clear();
    return 0;
};

int field_t::rotate_counterclockwise() {
    remove_previous();
    if (is_rotation_possible(current_figure->current_pos->prev_pos))
        current_figure->rotate_counterclockwise();
    recompose();
    new_rectangles.clear();
    return 0;
};

figure_position_t* field_t::next_figure() {
    DEBUG_VAR("%d\n", NUM_FIGURES);
    ENUM_FIGURES n = static_cast<ENUM_FIGURES> (rand() % static_cast<int>(NUM_FIGURES));
    int pos = rand() % FIG_POS_COUNTS[n];
    return &FIG_POSITIONS[n][pos];
};

bool field_t::is_figure_landed() {
    DEBUG_TRACE;
    if (is_landed) {
        DEBUG_VAR("%d\n", is_landed);
        return true;
    }
    int p_x = current_figure->pos_x;
    int p_y = current_figure->pos_y;
    if (p_y + current_figure->current_pos->size_y + 1 > SZ_Y) {
		DEBUG_PRINT("is_figure_landed:1:true\n");
        is_landed = true;
        return true;
    }
    for (int i=0; i < current_figure->current_pos->cnt_lower_points; ++i) {
    	char x = current_figure->current_pos->lower_points[i].x;
    	char y = current_figure->current_pos->lower_points[i].y;
    	if (fld[p_y + y + 1][p_x + x] == '1') {
    		DEBUG_PRINT("is_figure_landed:2:true\n");
            is_landed = true;
    		return true;
    	}
    }
	DEBUG_PRINT("is_figure_landed:false\n");
    is_landed = false;
    return false;
}

bool field_t::is_game_ended() {
    for (int i = 0; i < VIS_Y; ++i)
        for (int k = 0; k < SZ_X; ++k)
            if (fld[i][k] == '1')
                return true;
    return false;
}

int field_t::remove_line(int n_line) {
    for (int i = 0; i < SZ_X; ++i)
        fld[n_line][i] = '0';
    for (int i = n_line; i > VIS_Y; --i)
        for (int k = 0; k < SZ_X; ++k) {
            fld[i][k] = fld[i - 1][k];
            fld[i - 1][k] = '0';
        }
    return 0;
}

int field_t::remove_full_lines() {
    //int p_x = current_figure->pos_x;
    int p_y = current_figure->pos_y;
    int cnt_lines = 0;
    for (int i = p_y; i < p_y + current_figure->current_pos->size_y; ++i) {
        int sum = 0;
        for (int k = 0; k < SZ_X; ++k)
            if (fld[i][k] == '1')
                sum += 1;
        if (sum == SZ_X) {
            remove_line(i);
            ++cnt_lines;
        }
    }
    if (cnt_lines)
        set_redraw_flag();
    return cnt_lines;
}

bool field_t::is_rotation_possible(figure_position_t* candidate_pos) {
    int p_x = current_figure->pos_x;
    int p_y = current_figure->pos_y;
    // check x axis
    if (p_x + candidate_pos->size_x > SZ_X)
        return false;
    if (p_y + candidate_pos->size_y > SZ_Y)
        return false;
    for (int i = 0; i < candidate_pos->size_y; ++i)
        for (int k = 0; k < candidate_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] == '1' && candidate_pos->layout[i][k] == 1)
                return false;
    return true;
};
bool field_t::is_move_left_possible() {
    DEBUG_TRACE;
    int p_x = current_figure->pos_x - 1;
    int p_y = current_figure->pos_y;
    if (p_x < 0)
        return false;
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] == '1' && current_figure->current_pos->layout[i][k] == 1)
                return false;
    return true;
};

bool field_t::is_move_right_possible() {
    DEBUG_TRACE;
    int p_x = current_figure->pos_x + 1;
    int p_y = current_figure->pos_y;
    if (p_x + current_figure->current_pos->size_x > SZ_X)
        return false;
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] == '1' && current_figure->current_pos->layout[i][k] == 1)
                return false;
    return true;
};

int field_t::move_left() {
    DEBUG_TRACE;
    bool res;
    remove_previous();
    if ((res = is_move_left_possible())) {
        current_figure->pos_x -= 1;
        recompose();
        new_rectangles.clear();
        is_landed = false;
    }
    DEBUG_VAR("%d\n", res);
    return 0;
}

int field_t::move_right() {
    DEBUG_TRACE;
    bool res;
    remove_previous();
    if ((res = is_move_right_possible())) {
        current_figure->pos_x += 1;
        recompose();
        new_rectangles.clear();
        is_landed = false;
    }
    DEBUG_VAR("%d\n", res);
    return 0;
}

void field_t::print() {
    printf("Score: %Lu\n", get_points());
    next_position->render();
    printf("\n");
    for (int i = VIS_Y; i < SZ_Y; ++i) {
        for (int k = 0; k < SZ_X; ++k)
            printf("%c", fld[i][k]);
        printf("\n");
    }
    printf("-------------\n");
}

int field_t::x_setup(Display* dpy, Window* wnd, GC* gcb, GC* gcw, int fld_x, int fld_y, 
    int score_x, int score_y, int next_blk_x, int next_blk_y) {
    this->disp = dpy;
    this->wnd = wnd;
    this->gcblack = gcb;
    this->gcwhite = gcw;
    this->field_x = fld_x;
    this->field_y = fld_y;
    this->scor_x = score_x;
    this->scor_y = score_y;
    this->next_x = next_blk_x;
    this->next_y = next_blk_y;
    this->p_fontstruct = XQueryFont(dpy, XGContextFromGC(*gcw));
    this->font_height_px = p_fontstruct->ascent + p_fontstruct->descent;
    this->score_sz_px = 1;
    this->level_x = scor_x;
    this->level_y = scor_y + font_height_px;
    this->level_sz_px = 1;
    is_x = true;
    return 0;
}

int field_t::x_draw_empty_field() {
    // Border
    XDrawRectangle(disp, *wnd, *gcwhite, field_x, field_y, X_BLOCK_SZ * (SZ_X + 2), 
        X_BLOCK_SZ * (SZ_Y - VIS_Y + 2));
    XDrawRectangle(disp, *wnd, *gcwhite, field_x + X_BLOCK_SZ - 1, field_y + X_BLOCK_SZ - 1, 
        X_BLOCK_SZ * SZ_X + 1, X_BLOCK_SZ * (SZ_Y - VIS_Y) + 1);
    // Flush field
    XFillRectangle(disp, *wnd, *gcblack, field_x + X_BLOCK_SZ, field_y + X_BLOCK_SZ, 
        X_BLOCK_SZ * SZ_X, X_BLOCK_SZ * (SZ_Y - VIS_Y));
    // Flush score
    XFillRectangle(disp, *wnd, *gcblack, scor_x, scor_y, score_sz_px,
        font_height_px);
    // Flush level
    XFillRectangle(disp, *wnd, *gcblack, level_x, level_y, level_sz_px,
        font_height_px);
    // Flush next figure
    XFillRectangle(disp, *wnd, *gcblack, next_x, next_y, X_BLOCK_SZ * 4, 
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
            if (fld[y][x] == '1') {
                XFillRectangle(disp, *wnd, *gcwhite, t_fld_x + X_BLOCK_SZ * x, 
                    t_fld_y + X_BLOCK_SZ * (y - VIS_Y), X_BLOCK_SZ, X_BLOCK_SZ);
            }
        }
    }
    for (int y=0; y<next_position->size_y; ++y)
        for (int x=0; x<next_position->size_x; ++x)
            if (next_position->layout[y][x] == 1)
                XFillRectangle(disp, *wnd, *gcwhite, next_x + X_BLOCK_SZ * x, 
                    next_y + X_BLOCK_SZ * y, X_BLOCK_SZ, X_BLOCK_SZ);
    char s_buf[255];
    sprintf(s_buf, "Score: %Lu", get_points());
    score_sz_px = XTextWidth(p_fontstruct, s_buf, strlen(s_buf));
    XDrawString(disp, *wnd, *gcwhite, scor_x, scor_y + p_fontstruct->ascent, s_buf, strlen(s_buf));
    sprintf(s_buf, "Level: %u", level);
    level_sz_px = XTextWidth(p_fontstruct, s_buf, strlen(s_buf));
    XDrawString(disp, *wnd, *gcwhite, level_x, level_y + p_fontstruct->ascent, s_buf, strlen(s_buf));
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
    XFillRectangles(disp, *wnd, *gcblack, &(*(deleted_rectangles.begin())), deleted_rectangles.size());
    XFillRectangles(disp, *wnd, *gcwhite, &(*(new_rectangles.begin())), new_rectangles.size());
    XFlush(disp);
    prev_offset = cur_offset;
    new_rectangles.clear();
    deleted_rectangles.clear();
    return 0;
}

int field_t::x_render() {
    if (redraw_required)
        return x_redraw_full();
    return x_redraw_delta();
}

int field_t::x_set_rectangle_black(short x, short y) {
    DEBUG_TRACE;
    if (y < VIS_Y)
        return 0;

    XRectangle t {(short)(field_x + X_BLOCK_SZ * (x + 1)),
    	(short)(field_y + X_BLOCK_SZ * (y - VIS_Y + 1)), X_BLOCK_SZ, X_BLOCK_SZ};
    DEBUG_PRINT("x=%d, y=%d, t.x=%d, t.y=%d, t.width=%d, t.height=%d\n", x, y, t.x, t.y, t.width, t.height);
    deleted_rectangles.push_back(t);
    return 0;
}

int field_t::x_set_rectangle_white(short x, short y) {
    DEBUG_TRACE;
    if (y < VIS_Y)
        return 0;
    XRectangle t {(short)(field_x + X_BLOCK_SZ * (x + 1)),
    	(short)(field_y + X_BLOCK_SZ * (y - VIS_Y + 1)), X_BLOCK_SZ, X_BLOCK_SZ};
    DEBUG_PRINT("x=%d, y=%d, t.x=%d, t.y=%d, t.width=%d, t.height=%d\n", x, y, t.x, t.y, t.width, t.height);
    new_rectangles.push_back(t);
    return 0;
}

int field_t::x_fill_prev_black() {
    for (int i=0; i < prev_position->size_y; ++i) {
        if (prev_y + i < VIS_Y)
            continue;
        for (int k=0; k < prev_position->size_x; ++k) {
            if (prev_position->layout[i][k] == 1) {
                XRectangle t {(short)(field_x + X_BLOCK_SZ * (prev_x + k + 1)),
                    (short)(field_y + X_BLOCK_SZ * (prev_y - VIS_Y + i + 1) + prev_offset),
                    X_BLOCK_SZ, X_BLOCK_SZ};
                deleted_rectangles.push_back(t);
            }
        }
    }
	return 0;
}


int field_t::x_fill_cur_white() {
	for (int i=0; i < current_figure->current_pos->size_y; ++i) {
		if (current_figure->pos_y + i < VIS_Y)
			continue;
		for (int k=0; k < current_figure->current_pos->size_x; ++k) {
            if (current_figure->current_pos->layout[i][k] == 1) {
    			XRectangle t {(short)(field_x + X_BLOCK_SZ * (current_figure->pos_x + k + 1)),
    				(short)(field_y + X_BLOCK_SZ * (current_figure->pos_y + i - VIS_Y + 1) + cur_offset),
    				X_BLOCK_SZ, X_BLOCK_SZ};
    			new_rectangles.push_back(t);
            }
		}
	}
	return 0;
}

