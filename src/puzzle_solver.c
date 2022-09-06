#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "io.h"
#include "Stack.h"
#include "PriorityQueue.h"

#define ABS(A, B) ((A > B) ? A - B : B - A)

typedef struct state_node
{
    State state; 
    uint g; 
    uint h;
    byte blank_row, blank_col; // blank tile's coordinates
    struct state_node* parent; 
}* StateNode;

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
static StateNode init_state_node(State state, uint g, StateNode parent)
{
    StateNode new = malloc(sizeof(*new));
    assert(new != NULL);

    new->state = state; 
    new->g = g;
    new->parent = parent; 

    // find blank tile's coordinates
    new->blank_row = NULLKEY;
    for (byte i = 0; i < N; i++)
    {
        for (byte j = 0; j < N; j++)
        {
            if (state->puzzle[i][j] == BLANK)
            {
                new->blank_row = i;
                new->blank_col = j;
                break;
            }
        }
        if (new->blank_row != NULLKEY)
            break;
    }
    return new;
}

// frees allocated memory of a state
static void destroy_state(State state)
{
    if (state == NULL)
        return;
    for (int i = 0; i < N; i++)
        free(state->puzzle[i]);
    free(state->puzzle);
    free(state);
}

// destroys state node
static void destroy_state_node(StateNode node)
{
    if (node == NULL)
        return;
    destroy_state(node->state);
    node->state = NULL;
}

// destroys given puzzle
static void destroy_puzzle(void* puzzle)
{
    StateNode p = puzzle;
    if (p == NULL)
        return;
    destroy_state_node(p);
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
static StateNode new_move(StateNode parent, Move new_move)
{
    State new = init_state();

    // copy current state in new state
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            new->puzzle[i][j] = parent->state->puzzle[i][j];
    
    // save previous move that lead to current state to avoid moving blank back and forth
    Move previous_move = parent->state->move;

    byte x = parent->blank_row;
    byte y = parent->blank_col;

    // check if coordinates are valid to execute new_move
    if (new_move == UP && previous_move != DOWN && x >= 1)
    {
        new->puzzle[x][y] = new->puzzle[x-1][y];
        new->puzzle[x-1][y] = BLANK;
    }
    else if (new_move == RIGHT && previous_move != LEFT && y + 1 < N)
    {
        new->puzzle[x][y] = new->puzzle[x][y+1];
        new->puzzle[x][y+1] = BLANK;
    }
    else if (new_move == DOWN && previous_move != UP && x + 1 < N)
    {
        new->puzzle[x][y] = new->puzzle[x+1][y];
        new->puzzle[x+1][y] = BLANK;
    }
    else if (new_move == LEFT && previous_move != RIGHT && y >= 1)
    {
        new->puzzle[x][y] = new->puzzle[x][y-1];
        new->puzzle[x][y-1] = BLANK;
    }
    else
    {
        destroy_state(new);
        return NULL;
    }
    new->move = new_move;

    StateNode new_node = init_state_node(new, parent->g + 1, parent);

    // new node's heuristic is parent's heuristic changed based on made move
    new_node->h = parent->h + heuristic_fix(parent, new_node);

    return new_node;
}

// expands the parent state and inserts children in pq
static void expand_state(StateNode parent, PriorityQueue pq)
{
    StateNode child; 
    for (Move move = 0; move < 4; move++)
        if ((child = new_move(parent, move)) != NULL)
            pq_insert(pq, child);
}

// solves given puzzle using A* Search
void puzzle_solve(State initial, State goal)
{
    PriorityQueue pq;
    pq_init(&pq, compare_puzzles, destroy_puzzle);

    Stack stack;
    stack_init(&stack, destroy_puzzle);

    StateNode current = init_state_node(initial, 0, NULL);
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
                StateNode path[32]; // solution path array

                uint i = 0;
                for (; current->parent != NULL; current = current->parent)
                    path[i++] = current;
                path[i] = current;
                
                for (int j = i; j >= 0; j--)
                {
                    print_puzzle(path[j]->state);
                    destroy_state_node(path[j]);
                }
                printf("\b \b\b \nSolved puzzle in %u moves!\n", i);
            }
            break;
        }

        stack_push(stack, current);
        depth++;

        // expand current state
        expand_state(current, pq);
    }
    stack_destroy(stack);
    pq_destroy(pq);
    destroy_state(goal);
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