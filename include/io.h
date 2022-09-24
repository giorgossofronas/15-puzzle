#pragma once

#include <stdio.h>
#include "puzzle_solver.h"

// scans puzzle board
void scan_puzzle(State);

// prints given puzzle
void print_puzzle(State);

// prints puzzle solution path
void print_solution_path(StateNode);