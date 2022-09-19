# 8-Puzzle Solver

An A.I. solver made in C simulating the `8-Puzzle` game on terminal.<br>
Given an initial 3x3 puzzle, the aim is to reach a specific goal-puzzle in the minimum number of moves. <br>
The A.I. solver achieves that using [A* search](https://en.wikipedia.org/wiki/A*_search_algorithm#:~:text=*%2Dlike%20algorithm.-,Description,shortest%20time%2C%20etc.) and a [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) heuristic.

## How to Play
  - Type `0` to input your own initial puzzle 
  - Type `1` to generate a random initial puzzle<br>
 
Then press `ENTER` and the optimal solution will appear step-by-step in less than a second.

After that, the program will ask the user if they would like to play again. <br>
   - Type `Y` or `y` to play again
   - Type `N` or `n` to exit

## Project Structure

- The `include` directory contains all necessary header files. Among them, the `"common.h"` file contains useful typedefs and includes that are thoroughly used in each and every source code file. Some of the most-used typedefs are:
  ```c
  typedef unsigned int uint;
  typedef unsigned char byte;
  ```

- The `src` directory contains, among other files, the `puzzle_solver.c` file, in which the whole A.I. solver is built. To be more specific, the program uses an enum and two structs to represent the puzzle. <br>(*Note: **Move** enum and **State** struct are part of `"puzzle_solver.h"` file*)<br>
  - a simple **Move** enum:
  ```c
  typedef enum
  {
      UP, RIGHT, DOWN, LEFT, INVALID
  } Move;
  ```
  - the **State** struct, containing the puzzle-board itself and the move that led to this puzzle-state:
  ```c
  typedef struct state_struct
  {
      byte** puzzle; 
      Move move; 
  }* State;
  ```
  
  - the **StateNode** struct, containing a *State*, g and h used for depth and heuristic evaluation accordingly, the coordinates of the blank tile and a pointer to the parent StateNode:
  ```c
  typedef struct state_node
  {   
      State state; 
      uint g; 
      uint h;
      byte blank_row, blank_col; 
      struct state_node* parent; 
  }* StateNode;
  ```
  
## About the A.I. Solver
  The core of the A* Search algorithm involves:<br>
   - a [Priority Queue](https://github.com/giorgossofronas/Abstract-Data-Types/tree/main/modules/PriorityQueue) used to evaluate states and remove them in proper order
   - a [Stack](https://github.com/giorgossofronas/Abstract-Data-Types/tree/main/modules/Stack) used to store all removed StateNodes and de-allocate used memory
   - a constant while loop assuring that the priority queue isn't empty
   - state expansion, meaning creation of child-states based on all possible moves
   ```c
    while (!pq_is_empty(pq))
    {    
        current = pq_remove(pq);
        if (is_state_same(current->state, goal)) 
        {
            /* puzzle has been solved
            ...
            ...
            ...                      */
            break;
        }
        stack_push(stack, current);
        depth++;
        expand_state(current, pq);
    }
   ```
   
   Another thing worth mentioning is how the **state evaluation** works. Simply, the state with the lowest f has priority, with **f = g + h**.<br>
   - **g: node depth**, meaning number of moves needed to go from initial state to current
   - **h: heuristic evaluation**. This implementation uses a Manhattan distance heuristic:
   ```c
  // sum of the manhattan distances of each tile from its goal position 
  static uint heuristic(State state)
  {       
      uint sum_manh = 0; 

      for (byte i = 0; i < N; i++)
          for (byte j = 0; j < N; j++)
              if (state->puzzle[i][j] != BLANK) 
                 sum_manh += ABS(state->puzzle[i][j] / N, i) + ABS(state->puzzle[i][j] % N, j);
      return sum_manh;
  }
   ```
   <br>To improve time complexity, instead of constantly calling `heuristic(current_state)` for each newly created puzzle, we use a different function to calculate the h value of a new puzzle.<br>When a new state is generated the previous (parent node's) heuristic changes only based on two puzzle-tiles:
<br>    (1) the blank one
<br>    (2) the one taking blank one's place<br><br>
 Therefore, instead of recalculating the whole puzzle's sum of manhattan distances(md),
   we just remove the md of the the two tiles, from the old puzzle, and add their new mds 
   in the h-sum.
   ```c
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
  
  // ...
  // usage in `new_move` function:
  
  new_node->h = parent->h + heuristic_fix(parent, new_node);
   ```
   
For more details, check out the **source code** over at the `src` directory.

## Compilation & Use
A `Makefile` is provided for Windows and Linux users.<br>
Simply run the command:
```bash 
$ make run
``` 
over at the project's directory to compile and run the program.

> This project was a bonus excercise of Assignment 1 of the course<br> [Data Structures](https://www.di.uoa.gr/en/studies/undergraduate/34), DIT-UoA 2022,
Giorgos Sofronas.
