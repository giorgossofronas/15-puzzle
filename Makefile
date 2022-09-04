# paths
MOD = ./modules
INCLUDE = ./include
SRC = ./src

# compiler
CC = gcc

# compiler options
CFLAGS = -Wall -Werror -g3 -I$(INCLUDE)

OBJS = $(MOD)/PriorityQueue.o $(MOD)/Stack.o $(SRC)/puzzle.o $(SRC)/io.o $(SRC)/main.o

EXEC = 8Puzzle

# compile executable
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(CFLAGS)

# cleaning
PHONY clean:
	@rm -f $(OBJS) $(EXEC)

# compile and run
run: $(EXEC)
	time ./$(EXEC)

valgrind: $(EXEC)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXEC)