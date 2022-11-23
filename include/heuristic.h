#pragma once

#include "puzzle_solver.h"

// sum of the manhanttan distances of each tile from its goal position 
u_int8_t manhattan_distance(State);

// puzzle's linear conflicts
u_int8_t linear_conflicts(State);