#ifndef AVLTREE_H
#define AVLTREE_H

typedef struct AVLNode {
    int station_id;           // Identifiant unique de la station
    long capacity;            // Capacité en kWh
    long load;     // Somme des consommations associées
    struct AVLNode* left;     // Sous-arbre gauche
    struct AVLNode* right;    // Sous-arbre droit
    int height;               // Hauteur du nœud
} AVLNode;


typedef struct AVLTree {
    AVLNode *root;
} AVLTree;

AVLNode* createNode(int station_id, long capacity);
AVLNode* rotateRight(AVLNode* y);
AVLNode* rotateLeft(AVLNode* x);
AVLNode* insertNode(AVLNode* node, int station_id, long capacity);
int getHeight(AVLNode* node);
int getBalance(AVLNode* node);
int isBalanced(AVLNode* root);

#endif // AVLTREE_H
