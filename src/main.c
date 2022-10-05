#include "io.h"

int main(void)
{
    printf("\nThe 15-puzzle Solver!!!\n");

    printf("\n- Type 0 to give your own initial puzzle.\n");
    printf("- Type 1 to generate random initial puzzle.\n");

    char input;
    while ((input = getchar()) != EOF)
        if (input == '0' || input == '1')
            break;

    State initial = init_state();
    initial->parent = NULL;
    initial->move = NON_APPLICABLE;

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
        return EXIT_FAILURE;

    // find blank tile's coordinates in initial puzzle
    initial->blank_row = 0xF;
    for (u_int8_t i = 0; i < N && initial->blank_row == 0xF; i++)
    {
        for (u_int8_t j = 0; j < N; j++)
        {
            if (!initial->puzzle[i][j])
            {
                initial->blank_row = i;
                initial->blank_col = j;
                break;
            }
        }
    }

    printf("\nSolution:\n");
    print_puzzle(initial);

    puzzle_solve(initial);

    return EXIT_SUCCESS;
}