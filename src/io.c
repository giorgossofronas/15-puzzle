#include "common.h"
#include "io.h"

// scans puzzle
void scan_puzzle(State state) 
{
    printf("Please enter numbers 1-8 for number tiles and 0 for the blank one.\n");

    byte symbol;

    // flags for input validation
    byte isNumUsed[9] = { 0 };

    for (byte i = 0; i < N; i++) 
    {
        for (byte j = 0; j < N; j++) 
        {
            printf("  puzzle[%hhu][%hhu]: ", i, j);

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
                printf("  Invalid input. Enter a number from 0 to 8.\n");
                j--;
            }
        }
    }
    printf("\n");
}

// prints given puzzle
void print_puzzle(State state) 
{
    printf("\n");
    for(byte i = 0; i < N; i++) 
    {
        printf("+---+---+---+\n");
        for(byte j = 0; j < N; j++) 
        {
            if (state->puzzle[i][j] == BLANK)
                printf("|   ");
            else
                printf("| %hhu ", state->puzzle[i][j]);
        }
        printf("|\n");
    }
    printf("+---+---+---+\n      ▼");
}