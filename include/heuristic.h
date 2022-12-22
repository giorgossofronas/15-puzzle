#pragma once

#include "solver.h"

// sum of the manhanttan distances of each tile from its goal position 
uint manhattan_distance(State);

// puzzle's linear conflicts
uint linear_conflicts(State);