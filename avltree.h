#ifndef AVLTREE_H
#define AVLTREE_H

#include "station.h"

typedef struct AVLNode {
    int key;
    StationData *data;
    int height;
    struct AVLNode *left, *right;
} AVLNode;

typedef struct AVLTree {
    AVLNode *root;
} AVLTree;

AVLTree *create_avl_tree();
AVLNode *insert_avl_node(AVLNode *node, int key, StationData *data);
void free_avl_tree(AVLTree *tree);

#endif // AVLTREE_H
