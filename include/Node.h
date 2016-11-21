// structure to represent a node in the whole project.
typedef struct Node
{
	// a key that is used to display the node on the text output.
	// if key is '#', then node is not traversable.
	// if key is '-', then node is traversable and has not been visited.
	// if key is 'C', then node is traversable, has been visited and hence is put in the closed set.
	// if key is 'O', then node is traversable, has not been visited and is in the priority queue.
    char key;

    // used select the min from the preiority queue.
    // small value of priority -> higer priority.
    float priority;

    // distance of path from the start node to this node.
    int level;

    // number of neighbors a node has.
    // number of edges to a node.
    int num_neighbors;

    // array of pointers to neighbor nodes.
    struct Node** neighbors;

    // pointer to previous node in the generated path.
    struct Node* parent;

    // indices in the graph 2D array.
    int x;
    int y;
} Node;

// function to initialize a node.
int node_init(Node**, int, int);

// function to free a node.
int node_free(Node**);