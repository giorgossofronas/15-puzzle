#pragma once

#include "common.h"

#define BLANK 0
#define NULLKEY 0xFF

// puzzle's size
#define N 3

// possible moves
typedef enum
{
    UP, RIGHT, DOWN, LEFT, INVALID
} Move;

typedef struct state_struct
{
    byte** puzzle;
    Move move; // move that resulted in this state
}* State;

typedef struct state_node
{
    State state; 
    uint g; 
    uint h;
    byte blank_row, blank_col; // blank tile's coordinates
    struct state_node* parent; 
}* StateNode;

// allocates memory for a state
State init_state(void);

// generates a random puzzle
void random_puzzle(State);

// checks if puzzle is solvable
bool is_puzzle_solvable(State);

// solves given puzzle using A* Search
void puzzle_solve(State, State);