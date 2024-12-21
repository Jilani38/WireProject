#ifndef AVLTREE_USAGE_H
#define AVLTREE_USAGE_H

#include "avltree_utils.h"
#include "utils.h"

// Function to read station data from a file and insert into the AVL tree
AVLNode* readAndInsertStations(const char* filename, AVLNode* root);

// Function to add consumption data to a station within the AVL tree
void addConsumption(AVLNode* root, int station_id, long load);

// Function to read consumer data from a file and update station consumption in the AVL tree
void readAndAddConsumers(const char* filename, AVLNode* root);

// Function to perform an in-order traversal of the AVL tree and write data to a CSV file
void inOrderTraversalToCSV(AVLNode* root, FILE* file);

// Function to collect nodes for top and bottom analysis using min and max heaps
void collectTopAndBottomNodes(AVLNode* root, NodeCollection* minCollection, NodeCollection* maxCollection);

// Function to collect all nodes of the AVL tree into an array in-order
void collectNodesInOrder(AVLNode* root, NodeCollection* collection);

#endif // AVLTREE_USAGE_H