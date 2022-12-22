#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "io.h"

// scans puzzle
void scan_puzzle(State state) {
    printf("Please enter numbers 1-%u for number tiles and 0 for the blank one.\n", N * N - 1);

    // flags for input validation
    bool* is_num_used = calloc(N * N, sizeof(bool));
    assert(is_num_used != NULL);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("  puzzle[%d][%d]: ", i, j);

            Tile num;
            scanf("%hhu", &num);
            if (num < N * N) {
                // check if input is repeated
                if(!is_num_used[num]) {
                    state->puzzle[i][j] = num;
                    is_num_used[num] = true;
                }
                else {
                    printf("  Number %hhu is already used. Try again with different one.\n", num);
                    j--;
                }
            }
            else {
                printf("  Invalid input. Enter a number from 0 to %u.\n", N * N - 1);
                j--;
            }
        }
    }
    printf("\n");
    free(is_num_used);
}

// prints given puzzle
void print_puzzle(State state) {
    printf("\n");
    for(int i = 0; i < N; i++) {
        printf("+");
        for (int n = N; n > 0; n--)
            printf("----+");
        printf("\n");
        for(int j = 0; j < N; j++) {
            if (!state->puzzle[i][j])
                printf("|    ");
            else
                printf("| %2hhu ", state->puzzle[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (int n = N; n > 0; n--)
        printf("----+");
    printf("\n          ▼");
}

// Prints puzzle solution path
static void print_solution_path(State state) {
    if (state->parent == NULL)
        return;
    print_solution_path(state->parent);
    print_puzzle(state);
}

// prints puzzle's solution
void print_solution(State state) {
    print_solution_path(state);
    switch (state->g) {
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