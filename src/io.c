#include <stdio.h>
#include "common.h"
#include "io.h"

void scan_puzzle(State state) 
{
    state->move = INVALID;

    byte symbol;

    // flags for input validation
    byte isNumUsed[9] = { 0 };

    for(byte i = 0; i < N; i++) 
    {
        for(byte j = 0; j < N; j++) {
            printf("    puzzle[%hhu][%hhu]: ", i, j);

            scanf("%hhu", &symbol);

            // check if input is a blank character or is a number greater than 0 and less than 9
            if(symbol >= 0 && symbol < N * N) {
                // check if input is repeated
                if(!isNumUsed[symbol]) 
                {
                    state->puzzle[i][j] = symbol;
                    isNumUsed[symbol] = 1;
                }
                else 
                {
                    printf("    ERROR: Number %hhu is already used. Try again with different input.\n", symbol);
                    j--;
                }
            }
            else {
                printf("    ERROR: Invalid input. Enter a number from 0 to 8.\n");
                j--;
            }
        }
    }
    printf("\n");
}

void print_puzzle(State state) 
{
    for(byte i = 0; i < N; i++) 
    {
        printf("+---+---+---+\n");
        for(byte j = 0; j < N; j++) 
            printf("| %hhu ", state->puzzle[i][j]);
        printf("|\n");
    }
    printf("+---+---+---+\n->\n");
}