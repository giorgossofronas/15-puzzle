#include <assert.h>
#include "common.h"
#include "io.h"

// scans puzzle
void scan_puzzle(State state) 
{
    printf("Please enter numbers 1-%u for number tiles and 0 for the blank one.\n", N * N - 1);

    // flags for input validation
    u_int8_t* isNumUsed = calloc(N * N, 1);
    assert(isNumUsed != NULL);

    for (u_int8_t i = 0; i < N; i++) 
    {
        for (u_int8_t j = 0; j < N; j++) 
        {
            printf("  puzzle[%hhu][%hhu]: ", i, j);

            u_int8_t symbol;
            if (!scanf("%hhu", &symbol))
                exit(EXIT_FAILURE);

            if (symbol < N * N) 
            {
                // check if input is repeated
                if(!isNumUsed[symbol]) 
                {
                    state->puzzle[i][j] = symbol;
                    isNumUsed[symbol] = 1;
                }
                else 
                {
                    printf("  Number %hhu is already used. Try again with different one.\n", symbol);
                    j--;
                }
            }
            else 
            {
                printf("  Invalid input. Enter a number from 0 to %u.\n", N * N - 1);
                j--;
            }
        }
    }
    printf("\n");
    free(isNumUsed);
}

// prints given puzzle
void print_puzzle(State state) 
{
    printf("\n");
    for(u_int8_t i = 0; i < N; i++) 
    {
        printf("+");
        for (u_int8_t n = N; n > 0; n--)
            printf("----+");
        printf("\n");
        for(u_int8_t j = 0; j < N; j++) 
        {
            if (!state->puzzle[i][j])
                printf("|    ");
            else
                printf("| %2hhu ", state->puzzle[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (u_int8_t n = N; n > 0; n--)
        printf("----+");
    printf("\n          ▼");
}

// Prints puzzle solution path
static void print_solution_path(State state)
{
    if (state->parent == NULL)
        return;
    print_solution_path(state->parent);
    print_puzzle(state);
}

void print_solution(State state)
{
    print_solution_path(state);
    switch (state->g)
    {
        case 0:
            printf("\b \b\b \nPuzzle already solved!\n");
            break;
        case 1:
            printf("\b \b\b \nSolved puzzle in 1 move!\n");
            break;
        default:
            printf("\b \b\b \nSolved puzzle in %u moves!\n", state->g);
            break;
    }
}