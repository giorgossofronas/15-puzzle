# paths
MODULES = ./modules
INCLUDE = ./include
SRC = ./src

MAKE += --silent

CC = gcc
EXEC = 15PuzzleSolver

# if on Windows
ifeq ($(OS), win) 
	CC = x86_64-w64-mingw32-gcc
	EXEC = 15PuzzleSolver.exe
endif

CFLAGS = -Wall -Werror -Wextra -O3 -I$(INCLUDE)

OBJS = $(MODULES)/Stack.o $(SRC)/heuristic.o $(SRC)/puzzle_solver.o $(SRC)/io.o $(SRC)/main.o

all:
	@$(MAKE) clean
	@$(MAKE) run

# compile executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(CFLAGS)

# cleaning
.PHONY: clean
clean:
	@$(RM) $(OBJS) $(EXEC)

# compile and run
run: $(EXEC)
	./$(EXEC)