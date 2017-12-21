#include <TetrisField.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>


TetrisField::TetrisField():
	current_figure(new TetrisFigure()),
	prev_figure(new TetrisFigure())
{
    DEBUG_TRACE;
    DEBUG_VAR("%d", SZ_Y);
    printf("%d\n", SZ_Y);
    for (int i = 0; i < SZ_Y; ++i) 
            for (int k = 0; k < SZ_X; ++k) 
                fld[i][k] = BLACK;
}

int TetrisField::remove_previous() {
    DEBUG_TRACE;
    for (int i = 0; i < (*prev_figure).current_pos->size_y; ++i)
        for (int k = 0; k < (*prev_figure).current_pos->size_x; ++k){
            if ((*prev_figure).current_pos->layout[i][k] == 1) {
                fld[(*prev_figure).pos_y + i][(*prev_figure).pos_x + k] = BLACK;
            }
        }
    DEBUG_TRACE;
    return 0;
}


int TetrisField::force_landing() {
    remove_previous();
    while(!is_figure_landed())
        ++((*current_figure).pos_y);
    recompose();
    return 0;
}

int TetrisField::recompose() {
    DEBUG_TRACE;
    if (!is_figure_landed()) {
    	set_prev_remove();
    	set_cur_new();
    }
    for (int i = 0; i < (*current_figure).current_pos->size_y; ++i)
        for (int k = 0; k < (*current_figure).current_pos->size_x; ++k)
            if ((*current_figure).current_pos->layout[i][k] == 1) {
                fld[(*current_figure).pos_y + i][(*current_figure).pos_x + k] = (*current_figure).color;
            }
    prev_figure->copy(current_figure.get());
    DEBUG_TRACE;
    return 0;
}


int TetrisField::rotate_clockwise() {
    remove_previous();
    if (is_rotation_possible((*current_figure).current_pos->next_pos))
        (*current_figure).rotate_clockwise();
    recompose();
    new_rectangles.clear();
    return 0;
};

int TetrisField::rotate_counterclockwise() {
    remove_previous();
    if (is_rotation_possible((*current_figure).current_pos->prev_pos))
        (*current_figure).rotate_counterclockwise();
    recompose();
    new_rectangles.clear();
    return 0;
};

bool TetrisField::is_figure_landed() {
    DEBUG_TRACE;
    int p_x = (*current_figure).pos_x;
    int p_y = (*current_figure).pos_y;
    if (p_y + (*current_figure).current_pos->size_y + 1 > SZ_Y) {
		DEBUG_PRINT("is_figure_landed:1:true\n");
        return true;
    }
    for (int i=0; i < (*current_figure).current_pos->cnt_lower_points; ++i) {
    	char x = (*current_figure).current_pos->lower_points[i].x;
    	char y = (*current_figure).current_pos->lower_points[i].y;
    	if (fld[p_y + y + 1][p_x + x] != BLACK) {
    		DEBUG_PRINT("is_figure_landed:2:true\n");
    		return true;
    	}
    }
	DEBUG_PRINT("is_figure_landed:false\n");
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
    //int p_x = (*current_figure).pos_x;
    int p_y = (*current_figure).pos_y;
    int cnt_lines = 0;
    for (int i = p_y; i < p_y + (*current_figure).current_pos->size_y; ++i) {
        int sum = 0;
        for (int k = 0; k < SZ_X; ++k)
            if (fld[i][k] != BLACK)
                sum += 1;
        if (sum == SZ_X) {
            remove_line(i);
            ++cnt_lines;
        }
    }
    return cnt_lines;
}

bool TetrisField::is_rotation_possible(TetrisFigurePosition* candidate_pos) {
    int p_x = (*current_figure).pos_x;
    int p_y = (*current_figure).pos_y;
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
    int p_x = (*current_figure).pos_x - 1;
    int p_y = (*current_figure).pos_y;
    if (p_x < 0)
        return false;
    for (int i = 0; i < (*current_figure).current_pos->size_y; ++i)
        for (int k = 0; k < (*current_figure).current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] != BLACK && (*current_figure).current_pos->layout[i][k] == 1)
                return false;
    return true;
};

bool TetrisField::is_move_right_possible() {
    DEBUG_TRACE;
    int p_x = (*current_figure).pos_x + 1;
    int p_y = (*current_figure).pos_y;
    if (p_x + (*current_figure).current_pos->size_x > SZ_X)
        return false;
    for (int i = 0; i < (*current_figure).current_pos->size_y; ++i)
        for (int k = 0; k < (*current_figure).current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] != BLACK && (*current_figure).current_pos->layout[i][k] == 1)
                return false;
    return true;
};

int TetrisField::move_left() {
    DEBUG_TRACE;
    bool res;
    remove_previous();
    if ((res = is_move_left_possible())) {
        (*current_figure).pos_x -= 1;
        recompose();
        new_rectangles.clear();
    }
    DEBUG_VAR("%d\n", res);
    return 0;
}

int TetrisField::move_right() {
    DEBUG_TRACE;
    bool res;
    remove_previous();
    if ((res = is_move_right_possible())) {
        (*current_figure).pos_x += 1;
        recompose();
        new_rectangles.clear();
    }
    DEBUG_VAR("%d\n", res);
    return 0;
}

//void TetrisField::print() {
//    printf("Score: %Lu\n", get_points());
//    next_position->render();
//    printf("\n");
//    for (int i = VIS_Y; i < SZ_Y; ++i) {
//        for (int k = 0; k < SZ_X; ++k)
//            printf("%d", fld[i][k]);
//        printf("\n");
//    }
//    printf("-------------\n");
//}

int TetrisField::clear_deleted_rectangles() {
	deleted_rectangles.clear();
	return 0;
}

int TetrisField::clear_new_rectangles() {
	new_rectangles.clear();
	return 0;
}

int TetrisField::set_prev_remove() {
    for (int i=0; i < (*prev_figure).current_pos->size_y; ++i) {
        if ((*prev_figure).pos_y + i < VIS_Y)
            continue;
        for (int k=0; k < (*prev_figure).current_pos->size_x; ++k) {
            if ((*prev_figure).current_pos->layout[i][k] == 1) {
                deleted_rectangles.emplace_back((*prev_figure).pos_y + i - VIS_Y, (*prev_figure).pos_x + k);
            }
        }
    }
	return 0;
}

int TetrisField::set_cur_new() {
	for (int i=0; i < (*current_figure).current_pos->size_y; ++i) {
		if ((*current_figure).pos_y + i < VIS_Y)
			continue;
		for (int k=0; k < (*current_figure).current_pos->size_x; ++k) {
            if ((*current_figure).current_pos->layout[i][k] == 1) {
    			new_rectangles.emplace_back((*current_figure).pos_y + i - VIS_Y, (*current_figure).pos_x + k);
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

std::unique_ptr<TetrisFigure> TetrisField::get_current_figure() {
	return std::move(current_figure);
}

void TetrisField::set_current_figure(std::unique_ptr<TetrisFigure>& p_new_cur) {
	prev_figure->copy(current_figure.get());
	auto figure_to_delete = std::move(current_figure);
	current_figure = std::move(p_new_cur);
	//is_figure_landed();
}

short TetrisField::get_cur_x() {
	return (*current_figure).pos_x;
}

short TetrisField::get_cur_y() {
	return (*current_figure).pos_y;
}

short TetrisField::get_prev_x() {
	return (*prev_figure).pos_x;
}

short TetrisField::get_prev_y() {
	return (*prev_figure).pos_y;
}

int TetrisField::turn() {
	prev_figure->copy(current_figure.get());
	remove_previous();
	++(current_figure->pos_y);
	recompose();
	DEBUG_VAR("%d\n", get_cur_x());
	DEBUG_VAR("%d\n", get_cur_y());
	DEBUG_VAR("%d\n", get_prev_x());
	DEBUG_VAR("%d\n", get_prev_y());
	return 0;
}

int TetrisField::get_field_size_x() {
	return SZ_X;
}

int TetrisField::get_field_size_y() {
	return SZ_Y;
}

short TetrisField::get_vis_y() {
	return VIS_Y;
}

ENUM_COLORS TetrisField::get_current_color() {
	return current_figure->color;
}
