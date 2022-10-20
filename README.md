# 15-Puzzle Solver

An A.I. solver made in C simulating the [15-Puzzle](https://en.wikipedia.org/wiki/15_puzzle) game on terminal.<br>
Given an initial 4x4 puzzle, the aim is to reach a specific goal-puzzle in the minimum number of moves.<br>
The A.I. solver achieves that using [IDA* search](https://en.wikipedia.org/wiki/Iterative_deepening_A*) and the sum of [Manhattan Distance](https://en.wikipedia.org/wiki/Taxicab_geometry) and [Linear Conflicts](https://medium.com/swlh/looking-into-k-puzzle-heuristics-6189318eaca2#:~:text=Linear%20Conflicts,reach%20their%20final%20goal%20position.) heuristic.

## How to Play

<img src="https://miro.medium.com/max/600/1*Kg5sN-5U3Q7hevW9qhz2rA.gif"
  alt="image_logo"
  align="right"
  width="175"
  height="175" />

  - Type `0` to input your own initial puzzle 
  - Type `1` to generate a random initial puzzle<br>
 
Then press `ENTER` and the optimal solution will appear step-by-step.

## Compilation & Use
A `Makefile` is provided for Windows and Linux users.<br>
Simply run the command:
```bash 
~$ make run
``` 
over at the project's directory to compile and run the program.

---
© Giorgos Sofronas
