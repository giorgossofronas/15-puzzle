#include <stdlib.h>
#include <assert.h>
#include "common.h"
#include "io.h"

// scans puzzle
void scan_puzzle(State state) 
{
    printf("Please enter numbers 1-%u for number tiles and 0 for the blank one.\n", N * N - 1);

    // flags for input validation
    byte* isNumUsed = calloc(N * N, 1);
    assert(isNumUsed != NULL);

    for (byte i = 0; i < N; i++) 
    {
        for (byte j = 0; j < N; j++) 
        {
            printf("  puzzle[%hhu][%hhu]: ", i, j);

            byte symbol;
            scanf("%hhu", &symbol);

            if (symbol >= 0 && symbol < N * N) 
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
    for(byte i = 0; i < N; i++) 
    {
        printf("+");
        for (byte n = N; n > 0; n--)
            printf("----+");
        printf("\n");
        for(byte j = 0; j < N; j++) 
        {
            if (state->puzzle[i][j] == BLANK)
                printf("|    ");
            else
                printf("| %2hhu ", state->puzzle[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (byte n = N; n > 0; n--)
        printf("----+");
    printf("\n        ");
    for (byte n = 3; n < N; n++)
        printf("  ");
    printf("▼");
}

// Prints puzzle solution path
void print_solution_path(StateNode puzzle)
{
    if (puzzle == NULL)
        return;
    print_solution_path(puzzle->parent);
    print_puzzle(puzzle->state);
}