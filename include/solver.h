#pragma once

#include <stdbool.h>

// simple typedef for unsigned integers
typedef unsigned int uint;

// puzzle's tile
typedef unsigned char Tile;

// puzzle size is NxN
#define N 4

// possible moves
typedef enum {
    LEFT, RIGHT, UP, DOWN, NONE
} Move;

typedef struct _state {
    Tile** puzzle; // puzzle-board
    uint g, h;
    uint blank_row, blank_col;
    Move move; // move that resulted in this state
    struct _state* parent;
}* State;

// allocates memory for a state
State init_state(void);

// generates a random puzzle
void random_puzzle(State);

// checks if puzzle is solvable
bool is_puzzle_solvable(State);

// solves puzzle using IDA*
void puzzle_solve(State);