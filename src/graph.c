#include <stdio.h>
#include <stdlib.h>

#include "../include/Errors.h"
#include "../include/Node.h"
#include "../include/Graph.h"

// utility funtion to run test
int graph_is_valid(int i, int j, int mh, int mw)
{
    if (i<0 || i>=mh)
        return 0;
    if (j<0 || j>=mw)
        return 0;
    return 1;
}

// graph test
int graph_test()
{
    char ar[3][3] = 
    {
        'a','b','c',
        'd','e','f',
        'g','h','i'
    };

    int h = 3, w = 3;

    Graph* alpha_graph = NULL;

    int err = 0;

    if (err = graph_init(&alpha_graph,h,w))
        printf("ERROR: %d\n", err);

    int i = 0, j = 0;

    int a = 0, b = 0;

    Node* tmp = NULL;

    for (i=0;i<h;++i)
    {
        for (j=0;j<w;++j)
        {
            if (err = node_init(&tmp,i,j))
                printf("ERROR: %d\n", err);

            tmp->key = ar[i][j];

            if (err = graph_add_node(alpha_graph, tmp, i, j))
                printf("ERROR: %d\n", err);
        }
    }

    for (i=0;i<h;++i)
    {
        for (j = 0;j<w;++j)
        {
            tmp = alpha_graph->nodes[i][j];
            for (a=-1;a<=1;++a)
            {
                for (b=-1;b<=1;++b)
                {
                    if (!(a==0 && b==0))
                    {
                        if (graph_is_valid(i+a,j+b,h,w))
                        {
                            if (err = graph_add_edge(tmp,alpha_graph->nodes[i+a][j+b]))
                            {
                                printf("ERROR: %d\n", err);
                            }
                        }
                    }
                }
            }
        }
    }

    for (i=0;i<h;++i)
    {
        for (j = 0;j<w;++j)
        {
            printf("%c: ", alpha_graph->nodes[i][j]->key);

            for (a=0;a< alpha_graph->nodes[i][j]->num_neighbors;++a)
            {
                printf("%c ", alpha_graph->nodes[i][j]->neighbors[a]->key);
            }
            printf("\n");
        }
    }

    if (err = graph_free(&alpha_graph))
        printf("ERROR: %d\n", err);

    return SUCCESS;
}

int graph_init(Graph** graph, int height, int width)
{
    Graph* new_graph = malloc(sizeof(Graph));

    // malloc failed, return error
    if (new_graph == NULL)
        return MEMERR;

    new_graph->height = height;
    new_graph->width = width;

    // allocate memory for storing pointer to array of pointers to arrays of pointers to nodes.
    new_graph->nodes = calloc(height, sizeof(Node**));
    
    // calloc failed, return error
    if (new_graph->nodes == NULL)
        return MEMERR;

    int i = 0, j = 0, err = 0, num_neighbors = 0;

    for (i = 0; i < height; ++i)
    {
        // allocate memory for pointer to array of pointers to nodes.
        new_graph->nodes[i] = calloc(width, sizeof(Node*));

        // calloc failed, return error
        if (new_graph->nodes[i] == NULL)
            return MEMERR;

        for (j = 0; j < width; ++j)
            new_graph->nodes[i][j] = NULL;
    }

    *graph = new_graph;

    return SUCCESS;
}

int graph_add_node(Graph* graph, Node* node, int i, int j)
{
    if (graph == NULL)
        return NULLPTR;

    graph->nodes[i][j] = node;
    return SUCCESS;
}

int graph_add_edge(Node* from_node, Node* to_node)
{
    if (from_node == NULL)
        return NULLPTR;

    // increment number of neighbors of from_node
    ++(from_node->num_neighbors);

    Node** ret_ptr = NULL;

    ret_ptr = realloc(from_node->neighbors, from_node->num_neighbors * sizeof(Node*));

    // realloc failed, return error
    if (ret_ptr == NULL)
    {
        --(from_node->num_neighbors);
        return MEMERR;
    }
    else
        from_node->neighbors = ret_ptr;

    if (from_node->neighbors == NULL)
        return NULLPTR;

    // add to_node as the alst elemnt in the neighbors array
    from_node->neighbors[(from_node->num_neighbors)-1] = to_node;
    return SUCCESS;
}

int graph_free(Graph** graph)
{
    int i = 0, j = 0;

    for (i = 0; i < (*graph)->height; ++i)
    {
        for (j = 0; j < (*graph)->width; ++j)
            node_free(&((*graph)->nodes[i][j]));
        free((*graph)->nodes[i]);
    }
    free((*graph)->nodes);
    free(*graph);

    *graph = NULL;

    return SUCCESS;
}