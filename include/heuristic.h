#pragma once

#include "puzzle_solver.h"

// sum of the manhanttan distances of each tile from its goal position 
uint manhattan_distance(State);

// puzzle's linear conflicts
uint linear_conflict(State);