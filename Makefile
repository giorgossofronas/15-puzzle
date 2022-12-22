# paths
INCLUDE = ./include
SRC = ./src

MAKE += --silent

# compiler options
CC = gcc
CFLAGS = -Wall -Werror -Wextra -g3 -O0 -I$(INCLUDE)

EXEC = 15PuzzleSolver

# if on Windows
ifeq ($(OS), win) 
	CC = x86_64-w64-mingw32-gcc
	EXEC = 15PuzzleSolver.exe
endif

# source and object files
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

all:
	@$(MAKE) clean
	@$(MAKE) run

# compile executable
$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# cleaning
clean:
	@$(RM) $(OBJS) $(EXEC)

# compile and run
run: $(EXEC)
	@./$(EXEC)
	@$(RM) $(SRC)/*.o

.PHONY: all clean run