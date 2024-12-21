#include <stdio.h>
#include <stdlib.h>
#include "avltree_usage.h"

// Function to read and insert stations into the AVL tree
AVLNode* readAndInsertStations(const char* filename, AVLNode* root) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening stations file");
        exit(EXIT_FAILURE);
    }

    int station_id;
    long capacity;

    // Read each line from the stations file and insert into the AVL tree
    while (fscanf(file, "%d;%ld", &station_id, &capacity) == 2) {
        root = insertNode(root, station_id, capacity);
    }

    fclose(file);
    return root;
}

// Function to add consumption to stations in the AVL tree
void addConsumption(AVLNode* root, int station_id, long load) {
    if (root == NULL) return;

    // Search for the station in the tree
    if (station_id < root->station_id) {
        addConsumption(root->left, station_id, load);
    } else if (station_id > root->station_id) {
        addConsumption(root->right, station_id, load);
    } else {
        // Add consumption when the station is found
        root->load += load;
        return;
    }
}

// Function to read consumer file and add their consumption to the AVL tree
void readAndAddConsumers(const char* filename, AVLNode* root) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening consumers file");
        exit(EXIT_FAILURE);
    }

    int station_id;
    long load;

    // Read each line from the consumers file
    while (fscanf(file, "%d;%ld", &station_id, &load) == 2) {
        // Add consumption to the corresponding station in the tree
        addConsumption(root, station_id, load);
    }

    fclose(file);
}

// Perform in-order traversal and write data to CSV
void inOrderTraversalToCSV(AVLNode* root, FILE* file) {
    if (root == NULL) return;

    // Traverse left subtree
    inOrderTraversalToCSV(root->left, file);

    // Write current node data to CSV file
    fprintf(file, "%d:%ld:%ld\n", root->station_id, root->capacity, root->load);

    // Traverse right subtree
    inOrderTraversalToCSV(root->right, file);
}

// Collect nodes for top and bottom analysis using min and max heaps
void collectTopAndBottomNodes(AVLNode* root, NodeCollection* minCollection, NodeCollection* maxCollection) {
    if (root == NULL) return;

    collectTopAndBottomNodes(root->left, minCollection, maxCollection);
    insertAsMin(minCollection, root);
    insertAsMax(maxCollection, root);
    collectTopAndBottomNodes(root->right, minCollection, maxCollection);
}

// Collect all nodes from the AVL tree into an array in-order
void collectNodesInOrder(AVLNode* root, NodeCollection* collection) {
    if (root == NULL) return;

    collectNodesInOrder(root->left, collection);
    addNodeToCollection(collection, root);
    collectNodesInOrder(root->right, collection);
}