// delay in 1/1000ths of a second.

#ifndef LOOP_DELAY
#define LOOP_DELAY 1
#endif

// Map structure to conviniently hold all data required fro A*
typedef struct Map
{
	// graph of all the pixels as nodes
    Graph* graph;
    // Start and End position for path-finding
    Node* start;
    Node* end;
    // open list as a priority queue implemented as a Min-Heap
    PriorityQueue* open_list;
    
    // pointers to gtk data requiredd to update GUI
    GdkPixbuf* pix_buf;
    GtkWidget* image;
} Map;

// Function to initialize the Map
int astar_map_init(Map**, GdkPixbuf*, GtkWidget*);

// Function to print the Map as text
void astar_print_map(Map*);

// Function to calculate the heuristic
float astar_heuristic(Node*, Node*);

// Function to check if given indices are valid for traversing on the map
int astar_is_valid(int, int, Map*);

// Function to add neighbors to the open_list 
int astar_add_neighbors_ol(Map*, Node*, Node*);