#include <cstdlib>
#include <cstdio>
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
    render();
    set_redraw_flag();
}

int field_t::render() {
    DEBUG_TRACE;
    DEBUG_VAR("%d\n", current_figure->current_pos->size_x);
    DEBUG_VAR("%d\n", current_figure->current_pos->size_y);
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
    remove_previous();
    if (is_figure_landed()) {
        render();
        points += remove_full_lines();
        delete current_figure;
        if (is_game_ended())
            return 1;
        prev_y = get_figure_start_position_y(prev_position);
        current_figure = new figure_t(next_position->figure_type, next_position, prev_y);
        prev_position = next_position;
        prev_x = 4;
        next_position = next_figure();
    }
    else {
        render();
        current_figure->pos_y += 1;
    }
    set_redraw_flag();
    DEBUG_TRACE;
    return 0;
}

int field_t::rotate_clockwise() {
    remove_previous();
    if (is_rotation_possible(current_figure->current_pos->next_pos))
        current_figure->rotate_clockwise();
    render();
    set_redraw_flag();
    return 0;
};

int field_t::rotate_counterclockwise() {
    remove_previous();
    if (is_rotation_possible(current_figure->current_pos->prev_pos))
        current_figure->rotate_counterclockwise();
    render();
    set_redraw_flag();
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
    int p_x = current_figure->pos_x;
    int p_y = current_figure->pos_y + 1;
    if (p_y + current_figure->current_pos->size_y > SZ_Y)
        return true;
    for (int i = 0; i < current_figure->current_pos->size_y; ++i)
        for (int k = 0; k < current_figure->current_pos->size_x; ++k)
            if (fld[p_y + i][p_x + k] == '1' && current_figure->current_pos->layout[i][k] == 1)
                return true;
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
    int p_x = current_figure->pos_x;
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
    if (res = is_move_left_possible()) {
        current_figure->pos_x -= 1;
        render();
    }
    set_redraw_flag();
    DEBUG_VAR("%d\n", res);
    return 0;
}

int field_t::move_right() {
    DEBUG_TRACE;
    bool res;
    remove_previous();
    if (res = is_move_right_possible()) {
        current_figure->pos_x += 1;
        render();
    }
    set_redraw_flag();
    DEBUG_VAR("%d\n", res);
    return 0;
}

void field_t::print() {
    if (!redraw_required)
        return;
    printf("Score: %Lu\n", get_points());
    next_position->render();
    printf("\n");
    for (int i = VIS_Y; i < SZ_Y; ++i) {
        for (int k = 0; k < SZ_X; ++k)
            printf("%c", fld[i][k]);
        printf("\n");
    }
    printf("-------------\n");
    redraw_required = false;
}