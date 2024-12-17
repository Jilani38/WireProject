#ifndef UTILS_H
#define UTILS_H
#include "avltree_utils.h"


typedef struct {
    AVLNode** nodes; // Tableau de pointeurs vers AVLNode
    int size;        // Nombre actuel de nœuds dans le tableau
    int capacity;    // Capacité maximale du tableau
} NodeArray;

typedef struct {
    AVLNode* heap[20]; // Tableau de pointeurs vers les nœuds AVL
    int size;          // Nombre d'éléments dans le tas
} NodeHeap;

NodeArray* initNodeArray(int capacity);
void addNodeToArray(NodeArray* array, AVLNode* node);
int compareByCapacity(const void* a, const void* b);
void printNodes(NodeArray* array);
void writeNodesToCSV(NodeArray* array, FILE* file);
void insertMaxHeap(NodeHeap* heap, AVLNode* station);
void insertMinHeap(NodeHeap* heap, AVLNode* station);
NodeHeap mergeHeaps(NodeHeap* minHeap, NodeHeap* maxHeap);
void writeHeapToCSV(const char* filename, NodeHeap* heap);
long calculateSurplus(AVLNode* node);
int compareBySurplus(const void* a, const void* b);
void sortHeapBySurplus(NodeHeap* heap);
void toUpperCase(char* str);


#endif // UTILS_H
