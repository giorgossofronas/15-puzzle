#include <assert.h>
#include <time.h>
#include <limits.h>
#include "io.h"
#include "heuristic.h"
#include "Stack.h"

// move applicability
static bool ap_ops[4][16] = {
    {0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}
};

// blank's offsets in each new move
static int8_t row_offset[] = {0, 0, -1, 1};
static int8_t col_offset[] = {-1, 1, 0, 0};

// initializes a new state
State init_state(void)
{
    State new = malloc(sizeof(*new));
    assert(new != NULL);
    return new;
}

// generates a new state, from parent based on new move
static State new_move(State parent, Move new_move)
{
    State new = init_state();

    // copy current state in new state
    for (u_int8_t i = 0; i < N; i++)
        for (u_int8_t j = 0; j < N; j++)
            new->puzzle[i][j] = parent->puzzle[i][j];

    // find blank's new coordinates
    new->blank_row = parent->blank_row + row_offset[new_move];
    new->blank_col = parent->blank_col + col_offset[new_move];

    // swap
    new->puzzle[parent->blank_row][parent->blank_col] = new->puzzle[new->blank_row][new->blank_col];
    new->puzzle[new->blank_row][new->blank_col] = 0;
    
    // assign values
    new->parent = parent;
    new->g = parent->g + 1;
    new->move = new_move;

    return new;
}

// returns heuristic of given state
static inline uint heuristic(State state)
{
    return manhattan_distance(state) + linear_conflict(state);
}

// opposite of given move
static inline Move opposite_move(Move move)
{
    return move & 1 ? move - 1 : move + 1;
}

// recursive IDA*
static uint ida(State state, uint treshold, Stack visited)
{
    if (state->parent != NULL)
    {
        stack_push(visited, state);
        state->h = heuristic(state);
    }

    if (!state->h)
    {
        print_solution(state);
        return 0;
    }

    uint f = state->g + state->h;
    if (f > treshold)
        return f;

    uint min = UINT_MAX;

    u_int8_t val = state->blank_row * N + state->blank_col;

    for (Move move = 0; move < 4; move++)
    {
        // out of bounds
        if (!ap_ops[move][val])
            continue;

        // avoid going back-and-forth
        if (state->move == opposite_move(move))
            continue;

        uint temp = ida(new_move(state, move), treshold, visited);
        if (!temp)
            return 0;
        if (temp < min)
            min = temp;
    }
    return min;
}

// IDA* control loop
void puzzle_solve(State initial)
{
    Stack visited;
    stack_init(&visited, free);

    initial->g = 0;
    uint treshold = initial->h = heuristic(initial);

    while ((treshold = ida(initial, treshold, visited)))
        stack_destroy(visited); // always avoid overflow scenario

    stack_destroy(visited);
    free(visited);
    free(initial);
}

// checks if given puzzle is solvable
// inspired by: https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/
bool is_puzzle_solvable(State state)
{
    u_int8_t temp[N*N];
    for (u_int8_t i = 0; i < N; i++)
        for (u_int8_t j = 0; j < N; j++)
            temp[i*N+j] = state->puzzle[i][j];
    
    uint inv_count = 0;
    for (u_int8_t i = 0; i < N * N - 1; i++)
        for (u_int8_t j = i + 1; j < N * N; j++)
            if (temp[j] && temp[i] > temp[j]) 
                inv_count++;

    u_int8_t blank_row = 0;
    for (int8_t i = N - 1; i >= 0; i--)
        for (int8_t j = N - 1; j >= 0; j--)
            if (!state->puzzle[i][j])
                blank_row = N - i;
    
    return (blank_row & 1) ? !(inv_count & 1) : (inv_count & 1);
}

// generates a random puzzle
void random_puzzle(State state)
{
    srand(time(NULL));

    for (u_int8_t i = 0; i < N * N; i++)
        state->puzzle[i/N][i%N] = i;

    for (u_int8_t i = 0; i < N * N; i++)
    {
        u_int8_t new_pos = i + rand() / (RAND_MAX / (N * N - i) + 1);
        // swap
        u_int8_t temp = state->puzzle[new_pos/N][new_pos%N];
        state->puzzle[new_pos/N][new_pos%N] = state->puzzle[i/N][i%N];
        state->puzzle[i/N][i%N] = temp;
    }

    // produce at most one unsolvable random puzzle
    if (!is_puzzle_solvable(state))
    {
        /* switch two consecutive tiles to change number of inversions |
           tiles changed shouldn't contain the blank one */
        if (!state->puzzle[0][0])
        {
            u_int8_t temp = state->puzzle[0][1];
            state->puzzle[0][1] = state->puzzle[0][2];
            state->puzzle[0][2] = temp;
        }
        else if (!state->puzzle[0][1])
        {
            u_int8_t temp = state->puzzle[1][1];
            state->puzzle[1][1] = state->puzzle[1][0];
            state->puzzle[1][0] = temp;
        }
        else
        {
            u_int8_t temp = state->puzzle[0][1];
            state->puzzle[0][1] = state->puzzle[0][0];
            state->puzzle[0][0] = temp;
        }
    }
}