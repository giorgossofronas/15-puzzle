#include "puzzle.h"
#include "io.h"
#include <stdio.h>

int main(void)
{
    printf("\nHi! Welcome to the 8-puzzle game!\n");
    
    // scan initial state
    printf("Give initial state:\n");
    State initial = init_state(); 
    scan_puzzle(initial);

    State goal = init_state();
    byte c = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            goal->puzzle[i][j] = c++;
    
    // print initial and goal states
    printf("\nInitial State:\n");
    print_puzzle(initial);
    printf("\nGoal State:\n");
    print_puzzle(goal);
    printf("\nSOLUTION:\n");

    // solve the puzzle
    puzzle_solve(initial, goal);
    return 0;
}