#include <stdlib.h>
#include <assert.h>
#include "puzzle.h"
#include "io.h"
#include "Stack.h"
#include "PriorityQueue.h"
#include <stdio.h>

typedef struct state_node
{
    State state; // void* to previous struct
    uint g; // depth of current node, AKA moves made from initial to current state
    uint h;
    byte blank_x, blank_y;
    struct state_node* parent; // void* to parent state
}* StateNode;

#define ABS(A, B) ((A > B) ? A - B : B - A)

// manhantan distance heuristic
// sum of the manhanttan distances of each tile from its goal position 
static uint heuristic(State state)
{
    uint sum_manh = 0; // sum of manhanttan distances of misplaced tiles from their right coordinates in a goal state

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (state->puzzle[i][j] != BLANK) 
               sum_manh += ABS(state->puzzle[i][j] / N, i) + ABS(state->puzzle[i][j] % N, j);
    return sum_manh;
}

// return true/false depending on if states match
bool is_state_same(State one, State two)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (one->puzzle[i][j] != two->puzzle[i][j]) 
                return false;
    return true;
}

// frees allocated memory of a state
void destroy_state(State state)
{
    if (state == NULL || state->move < 0 || state->move > 3)
        return;
    for (int i = 0; i < N; i++)
        free(state->puzzle[i]);
    free(state->puzzle);
    free(state);
}

void destroy_state_node(StateNode node)
{
    if (node->blank_x < 0 || node->blank_y > 8)
        return;
    destroy_state(node->state);
    //free(node);
}

// returns a state void* to a new created state
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

// returns a StateNode void* to a new created StateNode
StateNode init_state_node(State state, uint g, uint h, StateNode parent)
{
    StateNode new = malloc(sizeof(*new));
    assert(new != NULL);

    new->state = state; // copies to new state both the array and move
    new->g = g;
    new->h = h;
    new->parent = parent; // connect new state with the parent one

    new->blank_x = NULLKEY;
    for (byte i = 0; i < N; i++)
    {
        for (byte j = 0; j < N; j++)
        {
            if (state->puzzle[i][j] == BLANK)
            {
                new->blank_x = i;
                new->blank_y = j;
                break;
            }
        }
        if (new->blank_x != NULLKEY)
            break;
    }

    return new;
}

// generates a new state, from current based on new_move
StateNode new_move(StateNode parent, Move new_move)
{
    State new = init_state();

    // copy current state in new state
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            new->puzzle[i][j] = parent->state->puzzle[i][j];
    
    // save previous move that lead to current state to avoid moving zero back and forth
    Move previous_move = parent->state->move;

    byte x = parent->blank_x;
    byte y = parent->blank_y;

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

    return init_state_node(new, parent->g + 1, heuristic(new), parent);
}

// expands the parent state
void expand_state(StateNode parent, PriorityQueue pq)
{
    StateNode child; 
    for (Move move = 0; move < 4; move++)
        if ((child = new_move(parent, move)) != NULL)
            pq_insert(pq, child);
}

// checks if given puzzle is solvable
bool is_puzzle_solvable(State state)
{
    byte* temp = malloc(9);
    assert(temp != NULL);

    int c = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            temp[c++] = state->puzzle[i][j];
    
    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++)
        for (int j = i+1; j < 9; j++)
            if (temp[j] && temp[i] && temp[i] > temp[j]) 
                inv_count++;

    free(temp);
    if (inv_count % 2) 
        return false;
    return true;
}

// destroys given puzzle
void destroy_puzzle(void* puzzle)
{
    StateNode p = puzzle;
    if (p == NULL)
        return;
    destroy_state_node(p);
    // free(puzzle);
}

// compares 2 puzzles based on their g+h evaluation
int compare_puzzles(void* puzzle1, void* puzzle2)
{
    StateNode p1 = puzzle1;
    StateNode p2 = puzzle2;
    int f1 = p1->g + p1->h;
    int f2 = p2->g + p2->h;
    return f2 - f1;
}

// solves given puzzle using A* algorithm; returns the optimal path
void puzzle_solve(State initial, State goal)
{
    // used to calculate g
    uint depth = 0;

    Stack stack;
    stack_init(&stack, destroy_puzzle);

    // initialize the priority queue
    PriorityQueue pq;
    pq_init(&pq, compare_puzzles, destroy_puzzle);

    // list with solution
    StateNode current = init_state_node(initial, 0, heuristic(initial), NULL);

    // insert initial state in PQ
    pq_insert(pq, current);

    // current state removed
    // while there are no nodes to expand 
    while (!pq_is_empty(pq))
    {
        stack_push(stack, current);
        // pop state with highest priority and add it to the list
        current = pq_remove(pq);

        // if current is goal state
        if (is_state_same(current->state, goal)) 
            break;
        
        depth++;
        // current state isn't the goal one
        // expand current state and insert child-states in PQ
        expand_state(current, pq);
    }
    
    if (depth == 0)
        return;
    else
    {
        StateNode* path = malloc((depth+1) * sizeof(StateNode));
        assert(path != NULL);

        uint i = 0;
        while (current->parent != NULL)
        {
            path[i++] = current;
            current = current->parent;
        }
        path[i] = current;
        // path is filled with correct moves, print it
        
        for (int j = i; j >= 0; j--)
            print_puzzle(path[j]->state);

        free(path);

        printf("\nSolved puzzle in %u moves!\n", i);
    }
    stack_destroy(stack);
    pq_destroy(pq);
}