#include <stdio.h>
#include <stdlib.h>

#include "../include/Errors.h"
#include "../include/Node.h"

int node_init(Node** node, int i, int j)
{
    // allocate memory for node.
    Node* new_node = malloc(sizeof(Node));

    // malloc failed, return error.
    if (new_node == NULL)
        return MEMERR;

    // initalize
    new_node->key = 0;
    new_node->priority = 0.0;
    new_node->level = 0;
    new_node->num_neighbors = 0;
    new_node->parent = NULL;
    new_node->neighbors = NULL;
    new_node->x = i;
    new_node->y = j;

    *node = new_node;

    return SUCCESS;
}

int node_free(Node** node)
{
    // free array
    free((*node)->neighbors);

    free(*node);
    *node = NULL;
    return SUCCESS;
}