#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

// Initialize NodeCollection
NodeCollection* initNodeCollection(int capacity) {
    NodeCollection* collection = malloc(sizeof(NodeCollection));
    if (!collection) return NULL;
    
    collection->nodes = malloc(capacity * sizeof(AVLNode*));
    if (!collection->nodes) {
        free(collection);
        return NULL;
    }
    collection->size = 0;
    collection->capacity = capacity;
    return collection;
}

// Add node to collection, resizing if necessary
void addNodeToCollection(NodeCollection* collection, AVLNode* node) {
    if (collection->size == collection->capacity) {
        collection->capacity *= 2;
        collection->nodes = realloc(collection->nodes, collection->capacity * sizeof(AVLNode*));
        if (!collection->nodes) exit(EXIT_FAILURE);
    }
    collection->nodes[collection->size++] = node;
}

// Comparison by capacity for sorting
int compareByCapacity(const void* a, const void* b) {
    const AVLNode* nodeA = *(AVLNode**)a;
    const AVLNode* nodeB = *(AVLNode**)b;
    
    if (nodeA->capacity < nodeB->capacity) return -1;
    if (nodeA->capacity > nodeB->capacity) return 1;
    return 0;
}

// Print nodes
void printNodes(NodeCollection* collection) {
    for (int i = 0; i < collection->size; ++i) {
        printf("Station ID: %d, Capacity: %ld, Load: %ld\n", 
               collection->nodes[i]->station_id, collection->nodes[i]->capacity, collection->nodes[i]->load);
    }
}

// Write to CSV
void writeCollectionToCSV(NodeCollection* collection, FILE* file) {
    for (int i = 0; i < collection->size; ++i) {
        fprintf(file, "%d:%ld:%ld\n", collection->nodes[i]->station_id, 
                                       collection->nodes[i]->capacity, 
                                       collection->nodes[i]->load);
    }
}

// Insert as if it were a max heap 
void insertAsMax(NodeCollection* collection, AVLNode* station) {
    if (collection->size < 10) {
        // If the collection hasn't reached its capacity, just add the station
        addNodeToCollection(collection, station);
    } else {
        // Find the index of the station with the lowest load in the collection
        // Since we want the maximum, we'll look for the minimum value to potentially replace
        int minIndex = 0;
        for (int i = 1; i < 10; i++) {
            if (collection->nodes[i]->load < collection->nodes[minIndex]->load) {
                minIndex = i;
            }
        }
        
        // If the new station's load is greater than the minimum load in our collection
        if (station->load > collection->nodes[minIndex]->load) {
            // Replace the node with the lowest load with our new station
            collection->nodes[minIndex] = station;
        }
        // No need to update size because we're maintaining a fixed size of 10
    }
}

// Insert as if it were a min heap 
void insertAsMin(NodeCollection* collection, AVLNode* station) {
    if (collection->size < 10) {
        // If the collection hasn't reached its capacity, just add the station
        addNodeToCollection(collection, station);
    } else {
        // Find the index of the station with the highest load in the collection
        // Since we want the minimum, we'll look for the maximum value to potentially replace
        int maxIndex = 0;
        for (int i = 1; i < 10; i++) {
            if (collection->nodes[i]->load > collection->nodes[maxIndex]->load) {
                maxIndex = i;
            }
        }
        
        // If the new station's load is less than the maximum load in our collection
        if (station->load < collection->nodes[maxIndex]->load) {
            // Replace the node with the highest load with our new station
            collection->nodes[maxIndex] = station;
        }
        // No need to update size because we're maintaining a fixed size of 10
    }
}


// Calculate surplus
long calculateSurplus(AVLNode* node) {
    return node->capacity - node->load;
}

// Compare by surplus for sorting
// Compare two nodes by their surplus
int compareBySurplus(const void* a, const void* b) {
    AVLNode* nodeA = *(AVLNode**)a;
    AVLNode* nodeB = *(AVLNode**)b;
    
    long surplusA = calculateSurplus(nodeA);
    long surplusB = calculateSurplus(nodeB);

    if (surplusA < surplusB) return -1;
    if (surplusA > surplusB) return 1;
    return 0;
}

// Sort by surplus
void sortCollectionBySurplus(NodeCollection* collection) {
    qsort(collection->nodes, collection->size, sizeof(AVLNode*), compareBySurplus);
}

// Convert to uppercase
void toUpperCase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}