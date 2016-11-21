// Structure to hold heap data.
typedef struct Heap
{
    // Array of pointers to Nodes.
    Node** data;

    // size of the heap.
    int size;
} Heap;

// tests to check if heap is working appropriately.
int heap_test();

// function to initialize the heap.
int heap_init(Heap**);

// funtion that gives the minimum node in the heap.
int heap_find_min(Heap*, Node**);

// function that inserts a node based on the priority into min-heap.
int heap_insert_min(Heap*, Node*);

// function to delete the minimum node and maintain heap property.
int heap_delete_min(Heap*);

// function to free momory occupied by the heap.
int heap_free(Heap**);