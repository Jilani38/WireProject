#include <stdio.h>
#include <stdlib.h>
#include "avltree_utils.h"
#include "avltree_usage.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// Function to create a new node with station details
AVLNode* createNode(int station_id, long capacity) {
    AVLNode* newNode = malloc(sizeof(AVLNode));
    if (!newNode) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->station_id = station_id;
    newNode->capacity = capacity;
    newNode->load = 0;  // Initially no consumption
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;  // New node starts with height 1
    return newNode;
}

// Function to get the height of a node
int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

// Function to calculate the balance factor of a node
int getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// Function to perform a right rotation
AVLNode* rotateRight(AVLNode* y) {
    // Get the left child of y, which will become the new root
    AVLNode* x = y->left;
    // Save the right child of x (T2), as it will be moved
    AVLNode* T2 = x->right;

    // Perform rotation:
    // Make y the new right child of x
    x->right = y;
    // Make T2 the new left child of y
    y->left = T2;

    // Update heights:
    // Height of y is now one plus the max height of its children
    y->height = 1 + MAX(getHeight(y->left), getHeight(y->right));
    // Height of x (new root) is updated similarly
    x->height = 1 + MAX(getHeight(x->left), getHeight(x->right));

    return x; // Return the new root of this subtree
}

// Function to perform a left rotation
AVLNode* rotateLeft(AVLNode* x) {
    // Get the right child of x, which will become the new root
    AVLNode* y = x->right;
    // Save the left child of y (T2), as it will be moved
    AVLNode* T2 = y->left;

    // Perform rotation:
    // Make x the new left child of y
    y->left = x;
    // Make T2 the new right child of x
    x->right = T2;

    // Update heights:
    // Height of x is now one plus the max height of its children
    x->height = 1 + MAX(getHeight(x->left), getHeight(x->right));
    // Height of y (new root) is updated similarly
    y->height = 1 + MAX(getHeight(y->left), getHeight(y->right));

    return y; // Return the new root of this subtree
}

// Insert a node into the AVL tree and rebalance
AVLNode* insertNode(AVLNode* node, int station_id, long capacity) {
    if (node == NULL) return createNode(station_id, capacity);

    // Standard BST insertion
    if (station_id < node->station_id)
        node->left = insertNode(node->left, station_id, capacity);
    else if (station_id > node->station_id)
        node->right = insertNode(node->right, station_id, capacity);
    else
        return node;  // Station already exists

    // Update height of the current node
    node->height = 1 + MAX(getHeight(node->left), getHeight(node->right));

    // Check balance to rebalance this subtree
    int balance = getBalance(node);
    if (balance > 1) {  // Left heavy
        if (station_id < node->left->station_id) {
            return rotateRight(node);  // Left-Left case
        } else {
            node->left = rotateLeft(node->left);
            return rotateRight(node);  // Left-Right case
        }
    }
    if (balance < -1) {  // Right heavy
        if (station_id > node->right->station_id) {
            return rotateLeft(node);  // Right-Right case
        } else {
            node->right = rotateRight(node->right);
            return rotateLeft(node);  // Right-Left case
        }
    }

    return node;
}

// Check if the AVL tree is balanced
int isBalanced(AVLNode* root) {
    if (root == NULL) return 1;

    int balance = getBalance(root);
    if (abs(balance) > 1) return 0;  // If balance factor > 1 or < -1, tree is not balanced

    // Recursively check if left and right subtrees are balanced
    return isBalanced(root->left) && isBalanced(root->right);
}


// Helper function to free an AVL tree recursively
void freeAVLTree(AVLNode* node) {
    if (node == NULL) return;

    freeAVLTree(node->left);
    freeAVLTree(node->right);
    free(node);
}