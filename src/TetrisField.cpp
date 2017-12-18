#include <TetrisField.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>

extern const char FIG_POS_COUNTS[];
extern TetrisFigurePosition* FIG_POSITIONS[]; 


TetrisField::TetrisField() {
    DEBUG_TRACE;
    DEBUG_VAR("%d", SZ_Y);
    printf("%d\n", SZ_Y);
    for (int i = 0; i < SZ_Y; ++i) 
            for (int k = 0; k < SZ_X; ++k) 
                fld[i][k] = BLACK;

    TetrisFigurePosition* n = next_figure();
    prev_y = get_figure_start_position_y(n);
    cur_color = next_color;
    current_figure = new TetrisFigure(n->figure_type, n, prev_y);
    prev_x = 4;
    prev_position = current_figure->current_pos;
    next_position = next_figure();
    recompose();
	set_redraw_flag();
	level = 1;
    cur_offset = 0;
    DEBUG_TRACE;
}

int TetrisField::remove_previous() {
    DEBUG_TRACE;
    for (int i = 0; i < prev_position->size_y; ++i)
        for (int k = 0; k < prev_position->size_x; ++k){
            if (prev_position->layout[i][k] == 1) {
                fld[prev_y + i][prev_x + k] = BLACK;
            }
        }
    DEBUG_TRACE;
    return 0;
}

int TetrisField::get_figure_start_position_y(TetrisFigurePosition* pos) {
    return VIS_Y - pos->size_y + 1;
}

void TetrisField::exit() {
    set_exit_flag();
}

void TetrisField::force_landing() {
    remove_previous();
    while(!is_figure_landed())
        ++(current_figure->pos_y);
    recompose();
}

int TetrisField::recompose() {
    DEBUG_TRACE;
    if (!is_figure_landed()) {
    	set_prev_remove();
    	set_cur_new();
    }
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (current_figure->current_pos->layout[i][k] == 1) {
                fld[current_figure->pos_y + i][current_figure->pos_x + k] = cur_color;
            }
    prev_y = current_figure->pos_y;
    prev_x = current_figure->pos_x;
    prev_position = current_figure->current_pos;
    DEBUG_TRACE;
    return 0;
}


ENUM_TIC_RESULT TetrisField::tic(double tic_ratio) {
    DEBUG_TRACE;
    ENUM_TIC_RESULT result = TIC_RESULT_PLAY_ANIMATION;
    if (to_exit)
        return TIC_RESULT_GAME_OVER;
    cur_offset = 0;
    if (is_figure_landed()) {
        recompose();
        points += remove_full_lines();
        delete current_figure;
        if (is_game_ended())
            return TIC_RESULT_GAME_OVER;
        prev_y = get_figure_start_position_y(prev_position);
        current_figure = new TetrisFigure(next_position->figure_type, next_position, prev_y);
        cur_color = next_color;
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
        result = TIC_RESULT_FIGURE_LANDED;
    }
    else {
		if (tic_ratio >= 1.0) {
			remove_previous();
			current_figure->pos_y += 1;
			recompose();
			result = TIC_RESULT_TURN;
		}
		else {
			recompose();
			result = TIC_RESULT_PLAY_ANIMATION;
		}
    }
#ifdef DEBUG
	print();
#endif
    DEBUG_TRACE;
    return result;
}

int TetrisField::rotate_clockwise() {
    remove_previous();
    if (is_rotation_possible(current_figure->current_pos->next_pos))
        current_figure->rotate_clockwise();
    recompose();
    new_rectangles.clear();
    return 0;
};

int TetrisField::rotate_counterclockwise() {
    remove_previous();
    if (is_rotation_possible(current_figure->current_pos->prev_pos))
        current_figure->rotate_counterclockwise();
    recompose();
    new_rectangles.clear();
    return 0;
};

TetrisFigurePosition* TetrisField::next_figure() {
    DEBUG_VAR("%d\n", NUM_FIGURES);
    int nxt_col = rand() % (NUM_COLORS - 1);
    next_color = static_cast<ENUM_COLORS> (nxt_col + 1);
    assert(next_color != BLACK);
    ENUM_FIGURES n = static_cast<ENUM_FIGURES> (rand() % static_cast<int>(NUM_FIGURES));
    int pos = rand() % FIG_POS_COUNTS[n];
    return &FIG_POSITIONS[n][pos];
};

bool TetrisField::is_figure_landed() {
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
    	if (fld[p_y + y + 1][p_x + x] != BLACK) {
    		DEBUG_PRINT("is_figure_landed:2:true\n");
            is_landed = true;
    		return true;
    	}
    }
	DEBUG_PRINT("is_figure_landed:false\n");
    is_landed = false;
    return false;
}

bool TetrisField::is_game_ended() {
    for (int i = 0; i < VIS_Y; ++i)
        for (int k = 0; k < SZ_X; ++k)
            if (fld[i][k] != BLACK)
                return true;
    return false;
}

int TetrisField::remove_line(int n_line) {
    for (int i = 0; i < SZ_X; ++i)
        fld[n_line][i] = BLACK;
    for (int i = n_line; i > VIS_Y; --i)
        for (int k = 0; k < SZ_X; ++k) {
            fld[i][k] = fld[i - 1][k];
            fld[i - 1][k] = BLACK;
        }
    return 0;
}

int TetrisField::remove_full_lines() {
    //int p_x = current_figure->pos_x;
    int p_y = current_figure->pos_y;
    int cnt_lines = 0;
    for (int i = p_y; i < p_y + current_figure->current_pos->size_y; ++i) {
        int sum = 0;
        for (int k = 0; k < SZ_X; ++k)
            if (fld[i][k] != BLACK)
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

bool TetrisField::is_rotation_possible(TetrisFigurePosition* candidate_pos) {
    int p_x = current_figure->pos_x;
    int p_y = current_figure->pos_y;
    // check x axis
    if (p_x + candidate_pos->size_x > SZ_X)
        return false;
    if (p_y + candidate_pos->size_y > SZ_Y)
        return false;
    for (int i = 0; i < candidate_pos->size_y; ++i)
        for (int k = 0; k < candidate_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] != BLACK && candidate_pos->layout[i][k] == 1)
                return false;
    return true;
};
bool TetrisField::is_move_left_possible() {
    DEBUG_TRACE;
    int p_x = current_figure->pos_x - 1;
    int p_y = current_figure->pos_y;
    if (p_x < 0)
        return false;
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] != BLACK && current_figure->current_pos->layout[i][k] == 1)
                return false;
    return true;
};

bool TetrisField::is_move_right_possible() {
    DEBUG_TRACE;
    int p_x = current_figure->pos_x + 1;
    int p_y = current_figure->pos_y;
    if (p_x + current_figure->current_pos->size_x > SZ_X)
        return false;
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] != BLACK && current_figure->current_pos->layout[i][k] == 1)
                return false;
    return true;
};

int TetrisField::move_left() {
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

int TetrisField::move_right() {
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

void TetrisField::print() {
    printf("Score: %Lu\n", get_points());
    next_position->render();
    printf("\n");
    for (int i = VIS_Y; i < SZ_Y; ++i) {
        for (int k = 0; k < SZ_X; ++k)
            printf("%d", fld[i][k]);
        printf("\n");
    }
    printf("-------------\n");
}

int TetrisField::clear_deleted_rectangles() {
	deleted_rectangles.clear();
	return 0;
}

int TetrisField::clear_new_rectangles() {
	new_rectangles.clear();
	return 0;
}

int TetrisField::set_prev_remove() {
    for (int i=0; i < prev_position->size_y; ++i) {
        if (prev_y + i < VIS_Y)
            continue;
        for (int k=0; k < prev_position->size_x; ++k) {
            if (prev_position->layout[i][k] == 1) {
                deleted_rectangles.emplace_back(prev_y + i - VIS_Y, prev_x + k);
            }
        }
    }
	return 0;
}

int TetrisField::set_cur_new() {
	for (int i=0; i < current_figure->current_pos->size_y; ++i) {
		if (current_figure->pos_y + i < VIS_Y)
			continue;
		for (int k=0; k < current_figure->current_pos->size_x; ++k) {
            if (current_figure->current_pos->layout[i][k] == 1) {
    			new_rectangles.emplace_back(current_figure->pos_y + i - VIS_Y, current_figure->pos_x + k);
            }
		}
	}
	return 0;
}

int TetrisField::get_fld_pnt(int x, int y) {
	return fld[y][x];
}

std::deque<FieldAddr_t>& TetrisField::get_new_rectangles() {
	return new_rectangles;
}

std::deque<FieldAddr_t>& TetrisField::get_deleted_rectangles() {
	return deleted_rectangles;
}

void TetrisField::set_redraw_flag() {
	DEBUG_PRINT("set_redraw\n");
	redraw_required = true;
};

void TetrisField::unset_redraw_flag() {
	redraw_required = false;
};

void TetrisField::set_exit_flag() {
	to_exit = true;
};

TetrisFigurePosition* TetrisField::get_next_figure() {
	return next_position;
};

ENUM_COLORS TetrisField::get_next_color() {
	return next_color;
};

unsigned TetrisField::get_level() {
	return level;
};

ENUM_COLORS TetrisField::get_cur_color() {
	return cur_color;
};

bool TetrisField::is_redraw_required() {
	return redraw_required;
};
