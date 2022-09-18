#include <stdlib.h>
#include <assert.h>
#include "PriorityQueue.h"

// Priority Queue implemented using binary heap

typedef struct heap_node
{
	void* value;
}* HeapNode;

struct priority_queue
{
    HeapNode array; // heap array
    uint size; // number of elements
    uint capacity; // max number of elements
    CompareFunc compare;
    DestroyFunc destroy;
};

// swaps two heap nodes
static void swap(HeapNode a, HeapNode b)
{
    HeapNode temp = a->value;
    a->value = b->value;
    b->value = temp;
}

// heapify using bubble-up operation
static void bubble_up(PriorityQueue pq, uint node_id)
{
    // if root
    if (node_id == 1)
        return;

    uint parent = node_id / 2; // node's parent. Nodes are 1-based

    // if father < node, swap and continue
    if (pq->compare(pq->array[parent].value, pq->array[node_id].value) <= 0)
    {
        swap(&pq->array[parent], &pq->array[node_id]);
        bubble_up(pq, parent);
    }
}

// heapify using bubble-down operation
static void bubble_down(PriorityQueue pq, uint node_id)
{
    // find node's children
    uint left_child = node_id * 2;
    uint right_child = left_child + 1;

    uint size = pq_size(pq);
    if (left_child > size)
        return;

    uint max_child = left_child;
    if (right_child <= size && pq->compare(pq->array[left_child].value, pq->array[right_child].value) < 0)
        max_child = right_child;

    // if node < max_child swap and continue
    if (pq->compare(pq->array[node_id].value, pq->array[max_child].value) < 0)
    {
        swap(&pq->array[node_id], &pq->array[max_child]);
        bubble_down(pq, max_child);
    }
}

// initializes an empty priority queue
void pq_init(PriorityQueue* pq, CompareFunc compare, DestroyFunc destroy)
{
    assert(compare != NULL);

    *pq = malloc(sizeof(struct priority_queue));
    assert(*pq != NULL);

    (*pq)->array = malloc(sizeof(struct heap_node) * HEAP_MIN_CAPACITY);
    assert((*pq)->array != NULL);

    (*pq)->size = 0;
    (*pq)->capacity = HEAP_MIN_CAPACITY;
    pq_set_compare(*pq, compare);
    pq_set_destroy(*pq, destroy);
}

// inserts given item in pq
void pq_insert(PriorityQueue pq, void* item)
{
    pq->size++;

    if (pq->size == pq->capacity)
    {
        // double heap's size
        HeapNode new_array = realloc(pq->array, pq->capacity * 2 * sizeof(*new_array));
        assert(new_array != NULL);

        pq->capacity *= 2;
        pq->array = new_array;
    }

    // insert at the end of heap
    pq->array[pq->size].value = item;

    // heapify
    bubble_up(pq, pq->size);
}

void* pq_remove(PriorityQueue pq)
{
    if (pq_is_empty(pq))
        return NULL;

    // save element to return it
    void* item = pq->array[1].value;
    pq->array[1].value = NULL;

    swap(&pq->array[1], &pq->array[pq->size]);

    pq->size--;

    // heapify
    bubble_down(pq, 1);

    return item;
}

// destroys given pq
void pq_destroy(PriorityQueue pq)
{
    if (pq->destroy != NULL)
    {
        for (uint i = 1; i <= pq->size; i++)
            pq->destroy(pq->array[i].value);
    }
    free(pq->array);
    free(pq);
}

// returns pq's size
uint pq_size(PriorityQueue pq)
{
    return pq->size;
}

// checks if pq is empty
bool pq_is_empty(PriorityQueue pq)
{
    return !pq_size(pq);
}

// sets as new CompareFunc of given priority the given one
void pq_set_compare(PriorityQueue pq, CompareFunc compare)
{
    pq->compare = compare;
}

// sets as new DestroyFunc of given priority queue the given one
void pq_set_destroy(PriorityQueue pq, DestroyFunc destroy)
{
    pq->destroy = destroy;
}