#include "puzzle.h"
#include "io.h"
#include <stdio.h>

int main(void)
{
    printf("\nWelcome to the 8-puzzle game!\n");
    printf("Type 0 to give your own initial puzzle\n");
    printf("or 1 to generate random initial puzzle.\n");
    
    char input;
    while ((input = getchar()) != EOF)
        if (input == '0' || input == '1')
            break;

    State initial = init_state();
    initial->move = INVALID;

    if (input == '1')
    {
        random_puzzle(initial);
        while (!is_puzzle_solvable(initial))
            random_puzzle(initial);
    }
    else if (input == '0')
    {
        // scan initial state
        printf("Give initial state:\n");
        scan_puzzle(initial);
        while (!is_puzzle_solvable(initial))
        {
            printf("\nGiven puzzle is unsolvable.\nGive a different one:\n");
            scan_puzzle(initial);
        }
    }
    else
        return 1;

    State goal = init_state();
    goal->move = INVALID;

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