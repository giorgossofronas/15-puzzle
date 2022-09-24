#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "io.h"
#include "puzzle_solver.h"
#include "Stack.h"
#include "PriorityQueue.h"

// | A - B |
#define ABS(A, B) ((A > B) ? A - B : B - A)

// initializes a new state
State init_state(void)
{
    State new = malloc(sizeof(*new));
    assert(new != NULL);

    new->puzzle = malloc(N * sizeof(byte*));
    assert(new->puzzle != NULL);

    for (int i = 0; i < N; i++)
    {
        new->puzzle[i] = malloc(N);
        assert(new->puzzle[i] != NULL);
    }
    return new;
} 

// initializes a new state node
static StateNode init_state_node(State state, StateNode parent, byte brow, byte bcol)
{
    StateNode new = malloc(sizeof(*new));
    assert(new != NULL);

    new->state = state; 
    new->g = parent != NULL ? parent->g + 1 : 0;
    new->parent = parent; 
    new->blank_row = brow;
    new->blank_col = bcol;
    
    return new;
}

// frees allocated memory of a state
static void destroy_state(State* state)
{
    if (*state == NULL)
        return;

    for (int i = 0; i < N; i++)
        free((*state)->puzzle[i]);
    free((*state)->puzzle);
    free(*state);
    state = NULL;
}

// destroys given puzzle
static void destroy_puzzle(void* puzzle)
{
    StateNode p = puzzle;
    if (p == NULL)
        return;

    destroy_state(&p->state);
    free(puzzle);
    puzzle = NULL;
}

/* compares 2 puzzles based on their f=g+h evaluation
   puzzle with lowest evaluation has priority */
static int compare_puzzles(void* puzzle1, void* puzzle2)
{
    StateNode p1 = puzzle1;
    StateNode p2 = puzzle2;

    int f1 = p1->g + p1->h;
    int f2 = p2->g + p2->h;
    
    return f2 - f1;
}

// sum of the manhanttan distances of each tile from its goal position 
static uint heuristic(State state)
{
    uint sum_manh = 0; 

    for (byte i = 0; i < N; i++)
        for (byte j = 0; j < N; j++)
            if (state->puzzle[i][j] != BLANK) 
               sum_manh += ABS(state->puzzle[i][j] / N, i) + ABS(state->puzzle[i][j] % N, j);
    return sum_manh;
}

/* When a new node is generated the heuristic evaluation changes only due to two tiles: 
   - the blank one
   - the one that took its place. 
   Thus, instead of recalculating the whole puzzle's sum of manhanttan distances(md),
   we just remove the md of the the two tiles from the old puzzle and add their new mds 
   in the sum */
static int heuristic_fix(StateNode old_puzzle, StateNode new_puzzle)
{
    byte x1 = old_puzzle->state->puzzle[new_puzzle->blank_row][new_puzzle->blank_col] / N;
    byte y1 = old_puzzle->state->puzzle[new_puzzle->blank_row][new_puzzle->blank_col] % N;

    byte x2 = new_puzzle->state->puzzle[old_puzzle->blank_row][old_puzzle->blank_col] / N;
    byte y2 = new_puzzle->state->puzzle[old_puzzle->blank_row][old_puzzle->blank_col] % N;

    int old_manh = ABS(x1, new_puzzle->blank_row) + ABS(y1, new_puzzle->blank_col);
    int new_manh = ABS(x2, old_puzzle->blank_row) + ABS(y2, old_puzzle->blank_col);

    return new_manh - old_manh;
}

// return true/false depending on if states match
static bool is_state_same(State one, State two)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (one->puzzle[i][j] != two->puzzle[i][j]) 
                return false;
    return true;
}

// generates a new state, from parent based on new move
static StateNode new_move(StateNode parent, Move new_move, byte blank_row, byte blank_col)
{
    State new = init_state();
    new->move = new_move;

    // copy current state in new state
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            new->puzzle[i][j] = parent->state->puzzle[i][j];

    // swap
    byte temp = new->puzzle[parent->blank_row][parent->blank_col];
    new->puzzle[parent->blank_row][parent->blank_col] = new->puzzle[blank_row][blank_col];
    new->puzzle[blank_row][blank_col] = temp;
    
    StateNode new_node = init_state_node(new, parent, blank_row, blank_col);

    // new node's heuristic is parent's heuristic changed based on made move
    new_node->h = parent->h + heuristic_fix(parent, new_node);

    return new_node;
}

// expands the parent state and inserts children in pq
static void expand_state(StateNode parent, PriorityQueue pq)
{ 
    // opposites of moves' default order
    Move opposite[] = {DOWN, LEFT, UP, RIGHT};

    // blank's offsets in each new move
    byte row_offset[] = {-1, 0, 1, 0};
    byte col_offset[] = {0, 1, 0, -1};

    for (Move move = UP; move != INVALID; move++)
    {
        // avoid going back-and-forth
        if (parent->state->move == opposite[move])
            continue;

        // new empty space coordinates based on move
        byte blank_row = parent->blank_row + row_offset[move];
        byte blank_col = parent->blank_col + col_offset[move];

        // out of board bounds
        if (blank_row < 0 || blank_row >= N || blank_col < 0 || blank_col >= N)
            continue;

        pq_insert(pq, new_move(parent, move, blank_row, blank_col));
    }
}

// solves given puzzle using A* Search
void puzzle_solve(State initial, State goal)
{
    // find blank tile's coordinates in initial puzzle
    byte row = NULLKEY, col;
    for (byte i = 0; i < N; i++)
    {
        for (byte j = 0; j < N; j++)
        {
            if (initial->puzzle[i][j] == BLANK)
            {
                row = i;
                col = j;
                break;
            }
        }
        if (row != NULLKEY)
            break;
    }

    // A* Search

    PriorityQueue pq;
    pq_init(&pq, compare_puzzles, destroy_puzzle);

    Stack stack;
    stack_init(&stack, destroy_puzzle);

    StateNode current = init_state_node(initial, NULL, row, col);
    current->h = heuristic(current->state);

    pq_insert(pq, current);

    uint depth = 0;

    // while there are no nodes to expand 
    while (!pq_is_empty(pq))
    {
        current = pq_remove(pq);

        if (is_state_same(current->state, goal)) 
        {
            if (depth == 0)
            {
                print_puzzle(initial);
                printf("\b \b\b \nPuzzle already solved!\n");
            }
            else if (depth == 1)
            {
                print_puzzle(initial);
                print_puzzle(goal);
                printf("\b \b\b \nSolved puzzle in 1 move!\n");
            }
            else
            {
                print_solution_path(current);
                printf("\b \b\b \nSolved puzzle in %u moves!\n", current->g);
            }
            break;
        }

        stack_push(stack, current);
        depth++;

        // expand current state
        expand_state(current, pq);
    }
    destroy_puzzle(current);
    destroy_state(&goal);
    pq_destroy(pq);
    stack_destroy(stack);
}

// checks if given puzzle is solvable
bool is_puzzle_solvable(State state)
{
    byte temp[N*N];

    int c = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            temp[c++] = state->puzzle[i][j];
    
    uint inv_count = 0;
    for (int i = 0; i < N * N - 1; i++)
        for (int j = i + 1; j < N * N; j++)
            if (temp[j] && temp[i] > temp[j]) 
                inv_count++;

    return !(inv_count % 2);
}

// generates a random puzzle
void random_puzzle(State state)
{
    srand(time(NULL));

    for (byte i = 0; i < N * N; i++)
        state->puzzle[i/N][i%N] = i;

    for (byte i = 0; i < N * N; i++)
    {
        uint new_pos = i + rand() / (RAND_MAX / (N * N - i) + 1);
        // swap
        byte temp = state->puzzle[new_pos/N][new_pos%N];
        state->puzzle[new_pos/N][new_pos%N] = state->puzzle[i/N][i%N];
        state->puzzle[i/N][i%N] = temp;
    }

    // produce at most one unsolvable random puzzle
    if (!is_puzzle_solvable(state))
    {
        /* switch two consecutive tiles to change number of inversions |
           tiles changed shouldn't contain the blank one */
        if (state->puzzle[0][0] == BLANK)
        {
            byte temp = state->puzzle[0][1];
            state->puzzle[0][1] = state->puzzle[0][2];
            state->puzzle[0][2] = temp;
        }
        else if (state->puzzle[0][1] == BLANK)
        {
            byte temp = state->puzzle[1][1];
            state->puzzle[1][1] = state->puzzle[1][0];
            state->puzzle[1][0] = temp;
        }
        else
        {
            byte temp = state->puzzle[0][1];
            state->puzzle[0][1] = state->puzzle[0][0];
            state->puzzle[0][0] = temp;
        }
    }
}