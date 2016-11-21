#include <stdio.h>
#include <stdlib.h>

#include "../include/Errors.h"
#include "../include/Node.h"
#include "../include/Heap.h"
#include "../include/PriorityQueue.h"

// TBD
int pq_test()
{
    return heap_test();
}

int pq_init(PriorityQueue** pq)
{
    PriorityQueue* new_pq = malloc(sizeof(PriorityQueue));

    // malloc failed, return error.
    if (new_pq == NULL)
        return MEMERR;

    int err = 0;

    // initialize the heap
    if (err = heap_init(&(new_pq->min_heap)))
        return err;

    new_pq->size = new_pq->min_heap->size;

    *pq = new_pq;

    return SUCCESS;
}

int pq_insert(PriorityQueue* pq, Node* new_node)
{
    int err = 0;

    // insert node into Min-Heap
    if (err = heap_insert_min(pq->min_heap, new_node))
        return err;
    // increment size if no error
    ++(pq->size);
    return SUCCESS;
}

int pq_pull_highest_priority_element(PriorityQueue* pq, Node** tmp)
{
    int err = 0;

    // High priority -> lowest value of Node->priority

    // store min element in tmp.
    if (err = heap_find_min(pq->min_heap, tmp))
        return err;

    // delete min element.
    if (err = heap_delete_min(pq->min_heap))
        return err;

    // decrement priority queue size.
    --(pq->size);

    return SUCCESS;
}