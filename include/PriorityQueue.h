#pragma once

#include "common.h"

#define HEAP_MIN_CAPACITY 10

typedef struct priority_queue* PriorityQueue;

// initializes an empty priority queue
void pq_init(PriorityQueue*, CompareFunc, DestroyFunc);

// inserts a new item in priority queue
void pq_insert(PriorityQueue, void*);

// removes and returns the item with highest priority
// returns NULL if priority queue is empty
void* pq_remove(PriorityQueue);

// destroys the given priority queue by freeing all nodes
void pq_destroy(PriorityQueue);

// returns the number of elements in the priority queue
uint pq_size(PriorityQueue);

// returns true/false depending on if priority queue is empty or not
bool pq_is_empty(PriorityQueue);

// sets as new CompareFunc of given priority queue the given one
void pq_set_compare(PriorityQueue, CompareFunc);

// sets as new DestroyFunc of given priority queue the given one
void pq_set_destroy(PriorityQueue, DestroyFunc);