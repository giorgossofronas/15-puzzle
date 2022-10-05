#pragma once

#include "common.h"

// puzzle size is NxN
#define N 4

// possible moves
typedef enum
{
    LEFT, RIGHT, UP, DOWN, NON_APPLICABLE
} Move;

typedef struct state_node
{
    u_int8_t puzzle[N][N];
    uint g;
    uint h;
    u_int8_t blank_row, blank_col;
    Move move; // move that resulted in this state
    struct state_node* parent;
}* State;

// allocates memory for a state
State init_state(void);

// generates a random puzzle
void random_puzzle(State);

// checks if puzzle is solvable
bool is_puzzle_solvable(State);

// solves puzzle using IDA*
void puzzle_solve(State);