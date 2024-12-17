#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"

// Initialisation d'un tableau dynamique
NodeArray* initNodeArray(int capacity) {
    NodeArray* array = (NodeArray*)malloc(sizeof(NodeArray));
    array->nodes = (AVLNode**)malloc(capacity * sizeof(AVLNode*));
    array->size = 0;
    array->capacity = capacity;
    return array;
}

// Ajouter un nœud au tableau dynamique
void addNodeToArray(NodeArray* array, AVLNode* node) {
    if (array->size >= array->capacity) {
        array->capacity *= 2;
        array->nodes = (AVLNode**)realloc(array->nodes, array->capacity * sizeof(AVLNode*));
    }
    array->nodes[array->size++] = node;
}



// Fonction de comparaison pour trier par capacité croissante
int compareByCapacity(const void* a, const void* b) {
    AVLNode* nodeA = *(AVLNode**)a;
    AVLNode* nodeB = *(AVLNode**)b;

    if (nodeA->capacity < nodeB->capacity) return -1; // a avant b
    if (nodeA->capacity > nodeB->capacity) return 1;  // a après b
    return 0; // a et b égaux
}

void printNodes(NodeArray* array) {
    for (int i = 0; i < array->size; i++) {
        AVLNode* node = array->nodes[i];
        printf("Station ID: %d, Capacity: %ld, Load: %ld\n", node->station_id, node->capacity, node->load);
    }
}

// Écrire les résultats dans un fichier CSV
void writeNodesToCSV(NodeArray* array, FILE* file) {
    // Écrire les données triées
    for (int i = 0; i < array->size; i++) {
        AVLNode* node = array->nodes[i];
        fprintf(file, "%d,%ld,%ld\n", node->station_id, node->capacity, node->load);
    }

}

void insertMaxHeap(NodeHeap* heap, AVLNode* node) {
    if (heap->size < 10) {
        // Ajouter directement si le tas n'est pas plein
        heap->heap[heap->size++] = node;
    } else {
        // Trouver l'élément avec la plus grande consommation
        int minIndex = 0;
        for (int i = 1; i < 10; i++) {
            if (heap->heap[i]->load < heap->heap[minIndex]->load) {
                minIndex = i;
            }
        }

        // Remplacer si la consommation du nouveau nœud est plus grande
        if (node->load > heap->heap[minIndex]->load) {
            heap->heap[minIndex] = node;
        }
    }
}


void insertMinHeap(NodeHeap* heap, AVLNode* node) {
    if (heap->size < 10) {
        // Ajouter directement si le tas n'est pas plein
        heap->heap[heap->size++] = node;
    } else {
        // Trouver l'élément avec la plus petite consommation
        int maxIndex = 0;
        for (int i = 1; i < 10; i++) {
            if (heap->heap[i]->load > heap->heap[maxIndex]->load) {
                maxIndex = i;
            }
        }

        // Remplacer si la consommation du nouveau nœud est plus petite
        if (node->load < heap->heap[maxIndex]->load) {
            heap->heap[maxIndex] = node;
        }
    }
}

NodeHeap mergeHeaps(NodeHeap* minHeap, NodeHeap* maxHeap) {
    // Initialiser un NodeHeap pour la fusion
    NodeHeap mergedHeap;
    mergedHeap.size = 0;

    // Allouer suffisamment d'espace pour tous les nœuds
    for (int i = 0; i < minHeap->size; i++) {
        mergedHeap.heap[mergedHeap.size++] = minHeap->heap[i];
    }

    for (int i = 0; i < maxHeap->size; i++) {
        mergedHeap.heap[mergedHeap.size++] = maxHeap->heap[i];
    }

    return mergedHeap;
}

void writeHeapToCSV(const char* filename, NodeHeap* heap) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier CSV");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Station LV:Capacity:Load (ALL)\n");

    for (int i = 0; i < heap->size; i++) {
        fprintf(file, "%d:%ld:%ld\n",
                heap->heap[i]->station_id,
                heap->heap[i]->capacity,
                heap->heap[i]->load);
    }

    fclose(file);
}



long calculateSurplus(AVLNode* node) {
    return labs(node->load - node->capacity); // labs pour le long, équivalent de abs
}

int compareBySurplus(const void* a, const void* b) {
    AVLNode* nodeA = *(AVLNode**)a;
    AVLNode* nodeB = *(AVLNode**)b;

    long surplusA = calculateSurplus(nodeA);
    long surplusB = calculateSurplus(nodeB);

    if (surplusA > surplusB) return -1; // Trier en ordre décroissant
    if (surplusA < surplusB) return 1;
    return 0;
}

void sortHeapBySurplus(NodeHeap* heap) {
    qsort(heap->heap, heap->size, sizeof(AVLNode*), compareBySurplus);
}

void toUpperCase(char* str) {
    while (*str) { // Parcourir la chaîne jusqu'au caractère nul '\0'
        *str = toupper((unsigned char)*str); // Convertir le caractère en majuscule
        str++; // Passer au caractère suivant
    }
}
