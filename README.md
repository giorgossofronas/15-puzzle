# 15-Puzzle

A command-line AI solver in C for the famous [15-Puzzle](https://en.wikipedia.org/wiki/15_puzzle) game.<br>
Given an initial puzzle configuration, the goal is to reach a specific final puzzle in the minimum number of moves. The solver uses the [IDA* search](https://en.wikipedia.org/wiki/Iterative_deepening_A*) algorithm, along with the sum of [Manhattan Distance](https://en.wikipedia.org/wiki/Taxicab_geometry) and [Linear Conflicts](https://medium.com/swlh/looking-into-k-puzzle-heuristics-6189318eaca2#:~:text=Linear%20Conflicts,reach%20their%20final%20goal%20position.) heuristic to find the optimal solution.

## Instructions
  To use the solver, run the program and follow the prompts:
  - Type `0` to input your own initial puzzle 
  - Type `1` to generate a random initial puzzle<br>
 
Then press `ENTER` and the optimal solution will be displayed step-by-step.

## Compilation & Use
**You can compile and run the solver online at [onlinegdb.com](https://www.onlinegdb.com/) by clicking [here](https://onlinegdb.com/ySc8Y49iD).**<br>
Note that the program may run slower online than it does on a local terminal.<br><br>
Alternatively a `Makefile` is provided for Windows and Linux users. To compile and run the program, navigate to the project's directory and run the following command:<br>
```bash 
~$ make run
``` 

---
© Giorgos Sofronas
