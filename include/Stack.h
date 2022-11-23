#pragma once

#include "common.h"

typedef struct stack_struct* Stack;

// initializes an empty stack
void stack_init(Stack*, DestroyFunc);

// pushes a new item on top of the stack
void stack_push(Stack, void*);

// pops the top item of the stack
// returns NULL if stack is empty
void* stack_pop(Stack);

// destroys the given stack by freeing all nodes
void stack_destroy(Stack);

// returns the number of elements in the stack
size_t stack_size(Stack);

// returns true/false depending on if stack is empty or not
bool stack_is_empty(Stack);

// returns the element at the top of the stack
// returns NULL if stack is empty
void* stack_top(Stack);

// sets as new DestroyFunc of given stack the given one
void stack_set_destroy(Stack, DestroyFunc);