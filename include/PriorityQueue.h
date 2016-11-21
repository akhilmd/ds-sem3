// Structure to fold Priority Queue data.
typedef struct PriorityQueue
{
	// Priority Queue is implemented as Min-Heap.
    Heap* min_heap;

    // Number of nodes in the Priority Queue.
    int size;
} PriorityQueue;

// function to test implementation of the Priority Queue.
int pq_test();

// function to initialize Priority Queue.
int pq_init(PriorityQueue**);

// funtion to insert into Priority Queue based on Node->priority
int pq_insert(PriorityQueue*, Node*);

// function to remove and return the highest Priority element
int pq_pull_highest_priority_element(PriorityQueue*,Node**);