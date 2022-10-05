#include <assert.h>
#include <limits.h>
#include "Stack.h"

typedef struct stack_node
{
    void* data;
    struct stack_node* next;
}* StackNode;

struct stack_struct
{
    StackNode top;
    u_int64_t size;
    DestroyFunc destroy;
};

// initializes an empty stack
void stack_init(Stack* stack, DestroyFunc destroy)
{
    *stack = malloc(sizeof(struct stack_struct));
    assert(*stack != NULL);

    (*stack)->top = NULL;
    (*stack)->size = 0;
    stack_set_destroy(*stack, destroy);
}

// pushes a new item on top of the stack
void stack_push(Stack stack, void* item)
{
    assert(stack != NULL);

    StackNode new = malloc(sizeof(struct stack_node));
    assert(new != NULL);

    stack->size++;
    new->data = item;
    new->next = stack->top;
    stack->top = new;
}

// pops the top item of the stack and returns a pointer to it
void* stack_pop(Stack stack)
{
    if (stack_is_empty(stack)) 
        return NULL;
    
    void* item = stack->top->data;
    StackNode new_top = stack->top->next;

    free(stack->top);
    stack->size--;

    stack->top = new_top;

    return item;
}

// destroys the given stack
void stack_destroy(Stack stack)
{
    if (stack_is_empty(stack))
    {
        // free(stack);
        return;
    }
     
    StackNode node = stack->top->next;
    while (!stack_is_empty(stack))
    {
        if (stack->destroy != NULL) 
            stack->destroy(stack->top->data);
        free(stack->top);

        stack->top = node;
        if (node != NULL) 
            node = node->next;

        stack->size--;
    }
    stack->top = NULL;
    // free(stack);
}

// returns the number of elements in the stack
u_int64_t stack_size(Stack stack)
{
    assert(stack != NULL);
    return stack->size;
}

// checks if stack is empty or not
bool stack_is_empty(Stack stack)
{
    assert(stack != NULL);
    return !stack_size(stack);
}

// returns the element at the top of the stack
void* stack_top(Stack stack)
{
    return (stack_is_empty(stack)) ? NULL : stack->top->data;
}

// sets as new DestroyFunc of given stack the given one
void stack_set_destroy(Stack stack, DestroyFunc destroy)
{
    assert(stack != NULL);
    stack->destroy = destroy;
}