#include <TetrisFigurePosition.h>

char FIG_POS_COUNTS[FIG_COUNT] = {NUM_FIG_POS_SQUARE, NUM_FIG_POS_Z, NUM_FIG_POS_BACK_Z,
                                            NUM_FIG_POS_R, NUM_FIG_POS_BACK_R, NUM_FIG_POS_STICK,
                                            NUM_FIG_POS_T};

struct TetrisFigurePosition SQUARE_POSITIONS[] = {
        {2, 2, FIG_SQUARE, {
            {1, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &SQUARE_POSITIONS[0],
        &SQUARE_POSITIONS[0],
        2, 2, 
        {{0, 0}, {1, 0}},
        {{0, 1}, {1, 1}}}, -1};

struct TetrisFigurePosition Z_POSITIONS[] = {
        {3, 2, FIG_Z, {
            {1, 1, 0, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &Z_POSITIONS[1],
        &Z_POSITIONS[1],
        3, 3, 
        {{0, 0}, {1, 0}, {2, 1}},
        {{0, 0}, {1, 1}, {2, 1}}, -1},
        {2, 3, FIG_Z, {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}},
        &Z_POSITIONS[0],
        &Z_POSITIONS[0],
        2, 2, 
        {{0, 1}, {1, 0}},
        {{0, 2}, {1, 1}}, -2}};

struct TetrisFigurePosition BACK_Z_POSITIONS[] = {
        {3, 2, FIG_BACK_Z, {
            {0, 1, 1, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &BACK_Z_POSITIONS[1],
        &BACK_Z_POSITIONS[1],
        3, 3, 
        {{0, 1}, {1, 0}, {2, 0}},
        {{0, 1}, {1, 1}, {2, 0}}, -1},
        {2, 3, FIG_BACK_Z, {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
        &BACK_Z_POSITIONS[0],
        &BACK_Z_POSITIONS[0],
        2, 2, 
        {{0, 0}, {1, 1}},
        {{0, 1}, {1, 2}}, -2}};

struct TetrisFigurePosition R_POSITIONS[] = {
        {3, 2, FIG_R, {
            {1, 0, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &R_POSITIONS[3],
        &R_POSITIONS[1],
        3, 3, 
        {{0, 0}, {1, 1}, {2, 1}},
        {{0, 1}, {1, 1}, {2, 1}}, -1},
        {2, 3, FIG_R, {
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}},
        &R_POSITIONS[0],
        &R_POSITIONS[2],
        2, 2, 
        {{0, 0}, {1, 0}},
        {{0, 2}, {1, 0}}, -2},
        {3, 2, FIG_R, {
            {1, 1, 1, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &R_POSITIONS[1],
        &R_POSITIONS[3],
        3, 3, 
        {{0, 0}, {1, 0}, {2, 0}},
        {{0, 0}, {1, 0}, {2, 1}}, -1},
        {2, 3, FIG_R, {
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}},
        &R_POSITIONS[2],
        &R_POSITIONS[0],
        2, 2, 
        {{0, 2}, {1, 0}},
        {{0, 2}, {1, 2}}, -2}};

struct TetrisFigurePosition BACK_R_POSITIONS[] = {
        {3, 2, FIG_BACK_R, {
            {0, 0, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &BACK_R_POSITIONS[3],
        &BACK_R_POSITIONS[1],
        3, 3, 
        {{0, 1}, {1, 1}, {2, 0}},
        {{0, 1}, {1, 1}, {2, 1}}, -1},
        {2, 3, FIG_BACK_R, {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 0, 0}},
        &BACK_R_POSITIONS[0],
        &BACK_R_POSITIONS[2],
        2, 2, 
        {{0, 0}, {1, 2}},
        {{0, 2}, {1, 2}}, -2},
        {3, 2, FIG_BACK_R, {
            {1, 1, 1, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &BACK_R_POSITIONS[1],
        &BACK_R_POSITIONS[3],
        3, 3, 
        {{0, 0}, {1, 0}, {2, 0}},
        {{0, 1}, {1, 0}, {2, 0}}, -1},
        {2, 3, FIG_BACK_R, {
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
        &BACK_R_POSITIONS[2],
        &BACK_R_POSITIONS[0],
        2, 2, 
        {{0, 0}, {1, 0}},
        {{0, 0}, {1, 2}}, -2}
};

struct TetrisFigurePosition STICK_POSITIONS[] = {
        {1, 4, FIG_STICK, {
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0},
            {1, 0, 0, 0}},
        &STICK_POSITIONS[1],
        &STICK_POSITIONS[1],
        1, 1, 
        {{0, 0}},
        {{0, 3}}, -3},
        {4, 1, FIG_STICK, {
            {1, 1, 1, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &STICK_POSITIONS[0],
        &STICK_POSITIONS[0],
        4, 4, 
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, 0}
};

struct TetrisFigurePosition T_POSITIONS[] = {
        {3, 2, FIG_T, {
            {0, 1, 0, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &T_POSITIONS[3],
        &T_POSITIONS[1],
        3, 3, 
        {{0, 1}, {1, 0}, {2, 1}},
        {{0, 1}, {1, 1}, {2, 1}}, -1},
        {2, 3, FIG_T, {
            {1, 0, 0, 0},
            {1, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 0}},
        &T_POSITIONS[0],
        &T_POSITIONS[2],
        2, 2, 
        {{0, 0}, {1, 1}},
        {{0, 2}, {1, 1}}, -2},
        {3, 2, FIG_T, {
            {1, 1, 1, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}},
        &T_POSITIONS[1],
        &T_POSITIONS[3],
        3, 3, 
        {{0, 0}, {1, 0}, {2, 0}},
        {{0, 0}, {1, 1}, {2, 0}}, -1},
        {2, 3, FIG_T, {
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 0, 0}},
        &T_POSITIONS[2],
        &T_POSITIONS[0],
        2, 2, 
        {{0, 1}, {1, 0}},
        {{0, 1}, {1, 2}}, -2}
};

struct TetrisFigurePosition* FIG_POSITIONS[FIG_COUNT] = {SQUARE_POSITIONS,
                                                        Z_POSITIONS, 
                                                        BACK_Z_POSITIONS, 
                                                        R_POSITIONS, 
                                                        BACK_R_POSITIONS, 
                                                        STICK_POSITIONS,
                                                        T_POSITIONS};

