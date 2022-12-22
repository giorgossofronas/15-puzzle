#include <assert.h>
#include <time.h>
#include <limits.h>
#include "io.h"
#include "heuristic.h"
#include "stack.h"

// initializes a new state
State init_state(void) {
    State new = malloc(sizeof(*new));
    assert(new != NULL);

    // init puzzle
    new->puzzle = malloc(N * sizeof(Tile*));
    assert(new->puzzle != NULL);
    for (int i = 0; i < N; i++) {
        new->puzzle[i] = malloc(N * sizeof(Tile));
        assert(new->puzzle[i] != NULL);
    }
    return new;
}

// returns heuristic of given state
static inline uint heuristic(State state) {
    return manhattan_distance(state) + linear_conflicts(state);
}

// generates a new state, from parent based on new move
static State new_move(State parent, Move new_move) {
    // blank's offsets in each new move
    static const int row_offset[] = {0, 0, -1, 1};
    static const int col_offset[] = {-1, 1, 0, 0};

    State new = init_state();
    // copy current state in new state
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
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
    new->h = heuristic(new);
    new->move = new_move;
    return new;
}

// opposite of given move
static inline Move opposite_move(Move move) {
    return move & 1 ? move - 1 : move + 1;
}

// move applicability
static const bool ap_ops[4][16] = {
    {0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}
};

// recursive IDA*
static uint ida(State state, uint treshold, Stack visited) {
    // add to frontier only if not initial state
    if (state->parent != NULL)
        stack_push(visited, state);

    // solved
    if (!state->h) {
        print_solution(state);
        return 0;
    }

    uint fscore = state->g + state->h;
    if (fscore > treshold)
        return fscore;

    // value of blank tile's pos (row, col) -> 1D-coordinate
    uint val_1d = state->blank_row * N + state->blank_col;

    uint min = UINT_MAX;
    for (Move move = 0; move < 4; move++) {
        // out of bounds
        if (!ap_ops[move][val_1d])
            continue;

        // avoid going back-and-forth
        if (state->move == opposite_move(move))
            continue;

        uint res = ida(new_move(state, move), treshold, visited);
        if (!res)
            return 0;
        else if (res < min)
            min = res;
    }
    return min;
}

// destroys given state
static void destroy_state(void* value) {
    State state = (State)value;
    for (int i = 0; i < N; i++)
        free(state->puzzle[i]);
    free(state->puzzle);
    free(state);
}

// IDA* control loop
void puzzle_solve(State initial) {
    /* frontier is a Stack (similarly to IDS), which is destroyed after each
       IDA* iteration, thus keeping only the visited states of current iteration */
    Stack visited;
    stack_init(&visited, destroy_state);

    uint treshold = initial->h = heuristic(initial);
    while ((treshold = ida(initial, treshold, visited)))
        stack_destroy(visited);

    destroy_state(initial);
    stack_destroy(visited);
    free(visited);
}

// checks if given puzzle is solvable
// inspired by: https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/
bool is_puzzle_solvable(State state) {
    // provided puzzle converted in 1D array
    Tile* puzzle_1d = malloc(N * N * sizeof(Tile));
    assert(puzzle_1d != NULL);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            puzzle_1d[i*N+j] = state->puzzle[i][j];
    
    uint inv_count = 0;
    for (int i = 0; i < N * N - 1; i++)
        for (int j = i + 1; j < N * N; j++)
            if (puzzle_1d[j] && puzzle_1d[i] > puzzle_1d[j]) 
                inv_count++;

    uint blank_row = 0;
    for (int i = N - 1; i >= 0; i--)
        for (int j = N - 1; j >= 0; j--)
            if (!state->puzzle[i][j])
                blank_row = N - i;

    free(puzzle_1d);
    
    return (blank_row & 1) ? !(inv_count & 1) : (inv_count & 1);
}

// generates a random puzzle
void random_puzzle(State state) {
    srand(time(NULL));
    for (int i = 0; i < N * N; i++)
        state->puzzle[i/N][i%N] = i;

    for (int i = 0; i < N * N; i++) {
        Tile new_pos = i + rand() / (RAND_MAX / (N * N - i) + 1);
        // swap
        Tile tmp = state->puzzle[new_pos/N][new_pos%N];
        state->puzzle[new_pos/N][new_pos%N] = state->puzzle[i/N][i%N];
        state->puzzle[i/N][i%N] = tmp;
    }

    // produce at most one unsolvable random puzzle
    if (!is_puzzle_solvable(state)) {
        /* switch two consecutive tiles to change number of inversions |
           tiles changed shouldn't contain the blank one */
        if (!state->puzzle[0][0]) {
            Tile tmp = state->puzzle[0][1];
            state->puzzle[0][1] = state->puzzle[0][2];
            state->puzzle[0][2] = tmp;
        }
        else if (!state->puzzle[0][1]) {
            Tile tmp = state->puzzle[1][1];
            state->puzzle[1][1] = state->puzzle[1][0];
            state->puzzle[1][0] = tmp;
        }
        else {
            Tile tmp = state->puzzle[0][1];
            state->puzzle[0][1] = state->puzzle[0][0];
            state->puzzle[0][0] = tmp;
        }
    }
}