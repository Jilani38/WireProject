#ifndef AVLTREE_USAGE_H
#define AVLTREE_USAGE_H
#include "avltree_utils.h"
#include "utils.h"

AVLNode* readAndInsertStations(const char* filename, AVLNode* root);
void addConsumption(AVLNode* root, int station_id, long load);
void readAndAddConsumers(const char* filename, AVLNode* root);
void inOrderTraversalToCSV(AVLNode* root, FILE* file);
void collectTopAndBottomNodes(AVLNode* root, NodeHeap* minHeap, NodeHeap* maxHeap);
void collectNodesInOrder(AVLNode* root, NodeArray* array);

#endif
