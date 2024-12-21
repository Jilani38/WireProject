#ifndef UTILS_H
#define UTILS_H

#include "avltree_utils.h"

// Combined structure for array and heap functionality
typedef struct {
    AVLNode** nodes;  // Array of pointers to AVL nodes
    int size;         // Current number of nodes 
    int capacity;     // Maximum capacity of the collection
} NodeCollection;

// Initialize a NodeCollection with given capacity
NodeCollection* initNodeCollection(int capacity);

// Add a node to the NodeCollection
void addNodeToCollection(NodeCollection* collection, AVLNode* node);

// Compare two nodes by their capacity
int compareByCapacity(const void* a, const void* b);

// Print all nodes in the NodeCollection
void printNodes(NodeCollection* collection);

// Insert a station node into the collection as if it were a max heap
void insertAsMax(NodeCollection* collection, AVLNode* station);

// Insert a station node into the collection as if it were a min heap
void insertAsMin(NodeCollection* collection, AVLNode* station);

// Write the contents of the collection to a CSV file
void writeCollectionToCSV(NodeCollection* collection, FILE* file);

// Calculate the surplus (capacity minus load) for a node
long calculateSurplus(AVLNode* node);

// Compare two nodes by their surplus
int compareBySurplus(const void* a, const void* b);

// Sort the collection by the surplus of nodes
void sortCollectionBySurplus(NodeCollection* collection);

// Convert a string to uppercase in-place
void toUpperCase(char* str);

#endif // UTILS_H