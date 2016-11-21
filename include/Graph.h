// structure of a graph.
typedef struct Graph
{
	// Pointer to a 2D array of pointers to nodes.
    Node*** nodes;

    // height and width of the graph.
    int height;
    int width;
} Graph;

// Some test funtions for the graph implementations.
int graph_test();

// initialize a graph.
int graph_init(Graph**, int,int);

// add a node to a graph.
int graph_add_node(Graph*, Node*, int, int);

// and an edge between two nodes.
int graph_add_edge(Node*, Node*);

// free the graph.
int graph_free(Graph**);