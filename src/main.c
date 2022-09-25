#include "io.h"

int main(void)
{
    char input;
    do
    {
        printf("\nTHE 8-PUZZLE SOLVER!!!\n");
        printf("- Type 0 to give your own initial puzzle.\n");
        printf("- Type 1 to generate random initial puzzle.\n");
        
        while ((input = getchar()) != EOF)
            if (input == '0' || input == '1')
                break;

        State initial = init_state();
        initial->move = INVALID;

        if (input == '1') // random initial puzzle
            random_puzzle(initial);
        else if (input == '0') // user's input puzzle
        {
            printf("Give initial puzzle:\n");
            scan_puzzle(initial);
            while (!is_puzzle_solvable(initial))
            {
                printf("\nGiven puzzle is unsolvable.\nGive a different one:\n");
                scan_puzzle(initial);
            }
        }
        else
            return 1;

        // initialize goal state
        State goal = init_state();
        goal->move = INVALID;
        byte c = 0;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                goal->puzzle[i][j] = c++;
        
        printf("\nSOLUTION:\n");
        puzzle_solve(initial, goal);

        printf("\nDo you want to play again? (Y/N)\n");
        while ((input = getchar()) != EOF)
            if (input == 'Y' || input == 'y' || input == 'N' || input == 'n')
                break;
    } 
    while (input != 'N' && input != 'n');

    printf("Thanks for playing!\n");
    return 0;
}