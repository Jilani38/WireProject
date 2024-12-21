#ifndef AVLTREE_H
#define AVLTREE_H

// Structure representing a node in the AVL tree
typedef struct AVLNode {
    int station_id;     // Unique identifier for the station
    long capacity;      // Capacity in kWh
    long load;          // Total consumption associated with the station
    struct AVLNode *left;   // Left subtree
    struct AVLNode *right;  // Right subtree
    int height;         // Height of the node
} AVLNode;

// Structure representing the AVL tree itself
typedef struct AVLTree {
    AVLNode *root;      // Pointer to the root node of the tree
} AVLTree;

// Function to create a new node with station data
AVLNode* createNode(int station_id, long capacity);

// Function to perform a right rotation
AVLNode* rotateRight(AVLNode* y);

// Function to perform a left rotation
AVLNode* rotateLeft(AVLNode* x);

// Function to insert a new node into the AVL tree and maintain balance
AVLNode* insertNode(AVLNode* node, int station_id, long capacity);

// Function to get the height of a node
int getHeight(AVLNode* node);

// Function to calculate the balance factor of a node
int getBalance(AVLNode* node);

// Function to check if the AVL tree is balanced
int isBalanced(AVLNode* root);

// Function to free an AVL tree recursively
void freeAVLTree(AVLNode* node);

#endif // AVLTREE_H