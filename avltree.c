#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"

// Fonctions AVL : création de nœud, rotation, insertion...

AVLNode *create_node(int key, StationData *data) {
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    node->key = key;
    node->data = data;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

// Autres fonctions AVL...
