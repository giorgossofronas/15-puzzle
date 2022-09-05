#pragma once

#include "common.h"

#define N 3 
#define BLANK 0
#define NULLKEY 0xFF

// possible moves
typedef enum
{
    UP, RIGHT, DOWN, LEFT, INVALID
} Move;

typedef struct state_struct
{
    byte** puzzle; // board NxN
    Move move; // move that resulted in this state
}* State;

// allocates memory for a state
State init_state(void);

// generates a random puzzle
void random_puzzle(State);

// checks if puzzle is solvable
bool is_puzzle_solvable(State);

// solves given puzzle using A* Search
void puzzle_solve(State, State);