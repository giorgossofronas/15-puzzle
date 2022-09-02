#pragma once

#include "common.h"
#include "PriorityQueue.h"

// board size is 3X3
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
    byte** puzzle; // current state / board NxN
    Move move; // move that resulted in this state
}* State;

// returns true/false depending on if states are equal
bool is_state_same(State, State);

// allocates memory for a state
State init_state(void);

// checks if puzzle is solvable
bool is_puzzle_solvable(State);

// solve given puzzle using A* algorithm, returns the path array
void puzzle_solve(State, State);