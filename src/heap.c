#include <stdio.h>
#include <stdlib.h>

#include "../include/Errors.h"
#include "../include/Node.h"
#include "../include/Heap.h"

int heap_test()
{
    // Heap tests.

    Heap* test_heap = NULL;

    int err = 0;
    Node* tmp = NULL;

    if (err = heap_init(&test_heap))
        printf("ERROR: %d\n", err);

    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);

    if (err = node_init(&tmp,-1,-1))
        printf("ERROR: %d\n", err);

    tmp->priority = 9.5;

    if (err = heap_insert_min(test_heap,tmp))
        printf("ERROR: %d\n", err);

    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);
    else
        printf("min: %f\n", tmp->priority);

    if (err = node_init(&tmp,-1,-1))
        printf("ERROR: %d\n", err);

    tmp->priority = 7.9;

    if (err = heap_insert_min(test_heap,tmp))
        printf("ERROR: %d\n", err);

    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);
    else
        printf("min: %f\n", tmp->priority);

    if (err = node_init(&tmp,-1,-1))
        printf("ERROR: %d\n", err);

    tmp->priority = 8.7;

    if (err = heap_insert_min(test_heap,tmp))
        printf("ERROR: %d\n", err);

    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);
    else
        printf("min: %f\n", tmp->priority);

    if (err = heap_delete_min(test_heap))
        printf("ERROR: %d\n", err);

    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);
    else
        printf("min: %f\n", tmp->priority);

    if (err = heap_delete_min(test_heap))
        printf("ERROR: %d\n", err);
    
    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);
    else
        printf("min: %f\n", tmp->priority);

    if (err = heap_delete_min(test_heap))
        printf("ERROR: %d\n", err);
    
    if (err = heap_find_min(test_heap, &tmp))
        printf("ERROR: %d\n", err);
    else
        printf("min: %f\n", tmp->priority);

    if (err = heap_free(&test_heap))
        printf("ERROR: %d\n", err);


    return SUCCESS;
}

int heap_init(Heap** heap)
{
    Heap* new_heap = malloc(sizeof(Heap));

    // malloc failed, return error.
    if (new_heap == NULL)
        return MEMERR;

    new_heap->data = NULL;
    new_heap->size = 0;

    *heap = new_heap;

    return SUCCESS;
}

int heap_find_min(Heap* heap, Node** tmp)
{
    if (heap == NULL)
        return NULLPTR;

    // if there is atleast one element in the heap
    if (heap->size > 0)
    {
        *tmp = heap->data[0];
        return SUCCESS;
    }

    // if no elements, return error
    return HEAP_EMPTY;
}

int heap_insert_min(Heap* heap, Node* new_node)
{
    // increase heap size.
    ++(heap->size);

    // return pointer for realloc
    Node** ret_ptr = NULL;

    // attempt to increase size of heap->data
    ret_ptr = realloc(heap->data, heap->size * sizeof(Node*));

    // if realloc failed, decrement size of heap
    // heap->data remains unchanged.
    // return error
    if (ret_ptr == NULL)
    {
        --(heap->size);
        return MEMERR;
    }
    // realloc successful
    else
        heap->data = ret_ptr;

    if (heap->data == NULL)
        return NULLPTR;

    int i = 0, p = 0;

    // from the last position in the heap.
    for (i=(heap->size)-1;i>=0;i=p)
    {
        // if root node, insert there.
        if (i==0)
        {
            heap->data[0] = new_node;
            break;
        }

        // index of parent node.
        p = (i - 1) / 2;

        // insert based on priority.
        if (heap->data[p]->priority > new_node->priority)
            heap->data[i] = heap->data[p];
        // correct position found, break and return success
        else
        {
            heap->data[i] = new_node;
            break;
        }
    }
    return SUCCESS;
}

int heap_delete_min(Heap* heap)
{
    int lc = 0, rc = 0, mn = 0, i =0;

    if (heap->data == NULL)
        return NULLPTR;

    // decrement heap size
    --(heap->size);

    // start from root
    for (i=0;i<(heap->size);)
    {
        // indices of left and right child in Heap binary tree
        lc = 2*i+1;
        rc = lc+1;

        // if left child exists
        if (lc<(heap->size))
        {
            mn = heap->data[lc]->priority < heap->data[(heap->size)]->priority ? lc : (heap->size);

            if (rc<(heap->size))
                mn = heap->data[mn]->priority < heap->data[rc]->priority ? mn : rc;

            // replace current node with smallest of last, left and right
            heap->data[i] = heap->data[mn];

            // do the same for the smallest of last, left and right
            if (i==mn)
                break;

            i=mn;
        }
        // if no children, then insert here.
        else
        {
            heap->data[i] = heap->data[(heap->size)];
            break;
        }
    }

    Node** ret_ptr = NULL;

    ret_ptr = realloc(heap->data, heap->size * sizeof(Node*));

    if (ret_ptr == NULL)
    {
        // realloc failed, return error.
        if (heap->size > 0)
        {
            ++(heap->size);
            return MEMERR;
        }
        // realloc succeded and heap is empty.
        heap->data = NULL;
        return SUCCESS;
    }
    // realloc succeded and heap is not empty.
    else
        heap->data = ret_ptr;
    
    return SUCCESS;
}

int heap_free(Heap** heap)
{
    int i = 0, err = 0;

    // free each node
    for (i=0;i<(*heap)->size;++i)
        if (err = node_free((*heap)->data[i]))
            return err;
    free(*heap);
    *heap = NULL;

    return SUCCESS;
}