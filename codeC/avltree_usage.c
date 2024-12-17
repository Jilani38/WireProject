#include <stdio.h>
#include <stdlib.h>
#include "avltree_usage.h"

// Fonction pour lire et ajouter les stations dans l'arbre AVL
AVLNode* readAndInsertStations(const char* filename, AVLNode* root) {
    // Vérification de l'ouverture du fichier contenant les stations
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des stations");
        exit(EXIT_FAILURE);
    }

    int station_id;
    long capacity;

    // Lecture de chaque ligne du fichier contenant les stations et ajout dans l'arbre AVL
    while (fscanf(file, "%d;%ld", &station_id, &capacity) == 2) {
        root = insertNode(root, station_id, capacity);
    }

    fclose(file);
    return root;
}


// Fonction pour ajouter la consommation aux stations
void addConsumption(AVLNode* root, int station_id, long load) {
    if (root == NULL) return;

    // Recherche de la station dans l'arbre
    if (station_id < root->station_id) {
        addConsumption(root->left, station_id, load);
    } else if (station_id > root->station_id) {
        addConsumption(root->right, station_id, load);
    } else {
        // Ajout de la consommation une fois la station trouvée
        root->load += load;
        return;
    }
}

// Fonction pour lire le fichier des consommateurs et ajouter leur consommation dans l'arbre AVL
void readAndAddConsumers(const char* filename, AVLNode* root) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des consommateurs");
        exit(EXIT_FAILURE);
    }

    int station_id;
    long load;
 

    // Lecture de chaque ligne du fichier des consommateurs
    while (fscanf(file, "%d;%ld", &station_id, &load) == 2) {
        // Ajout la consommation à la station correspondante dans l'arbre
        addConsumption(root, station_id, load);
    }

    fclose(file);
}



void inOrderTraversalToCSV(AVLNode* root, FILE* file) {
    if (root == NULL) return;

    // Parcours du sous-arbre gauche
    inOrderTraversalToCSV(root->left, file);

    // Écrire les données du nœud actuel dans le fichier CSV
    fprintf(file, "%d:%ld:%ld\n", root->station_id, root->capacity, root->load);

    // Parcours du sous-arbre droit
    inOrderTraversalToCSV(root->right, file);
}


void collectTopAndBottomNodes(AVLNode* root, NodeHeap* minHeap, NodeHeap* maxHeap) {
    if (root == NULL) return;

    // Parcours in-order
    collectTopAndBottomNodes(root->left, minHeap, maxHeap);

    // Ajouter le nœud actuel aux deux tas
    insertMinHeap(minHeap, root); // Pour les plus grandes consommations
    insertMaxHeap(maxHeap, root); // Pour les plus petites consommations

    collectTopAndBottomNodes(root->right, minHeap, maxHeap);
}

// Collecter les données de l'AVL dans un tableau
void collectNodesInOrder(AVLNode* root, NodeArray* array) {
    if (root == NULL) return;

    // Parcourir le sous-arbre gauche
    collectNodesInOrder(root->left, array);

    // Ajouter le nœud actuel au tableau
    addNodeToArray(array, root);

    // Parcourir le sous-arbre droit
    collectNodesInOrder(root->right, array);
}
