#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree_usage.h"
#include "avltree_utils.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    // Vérifier que les arguments nécessaires sont fournis
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <station_type> <consumer_type>\n", argv[0]);
        return 1;
    }

    char* station_type = argv[1];
    char* consumer_type = argv[2];

    // Construire le nom du fichier CSV
    char filename[256];
    snprintf(filename, sizeof(filename), "../tests/%s_%s.csv", station_type, consumer_type);

    // Vérification du cas lv all
    int is_lv_all;

    if (strcmp(consumer_type,"all")) {
        is_lv_all = 0;
    } else {
        is_lv_all = 1;
    }

    // Création de l'arbre AVL
    AVLNode* root = NULL;

    // Insérer les stations
    printf("Création de l'arbre AVL \n");
    root = readAndInsertStations("../tmp/stations_data.csv", root);


    // Lire et ajouter les consommateurs à l'arbre AVL
    printf("Ajout des consommations \n");
    readAndAddConsumers("../tmp/consumers_data.csv", root);


    // Initialiser le tableau dynamique pour collecter les données
    NodeArray* array = initNodeArray(100); // Capacité initiale de 100

    // Collecter les nœuds de l'AVL dans un tableau
    collectNodesInOrder(root, array);

    // Trier les nœuds par capacité croissante

    qsort(array->nodes, array->size, sizeof(AVLNode*), compareByCapacity);

    //Ouverture et initialisation du fichier de sortie
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier CSV");
        exit(1);
    }

    // Écrire les en-têtes
    toUpperCase(station_type);
    toUpperCase(consumer_type);
    fprintf(file, "Station %s,Capacity,Load (%s)\n", station_type, consumer_type);

    // Écrire les résultats dans un fichier CSV
    writeNodesToCSV(array, file);

    printf("Les résultats triés ont été écrits dans %s.\n", filename);

    // Libérer la mémoire et fermer le fichier
    fclose(file);
    free(array->nodes);
    free(array);

    // Pour le cas lv all blablablablabal
    if (is_lv_all) {
        // Initialiser les tas
        NodeHeap maxHeap = {{0}, 0}; // Pour les plus consommatrices
        NodeHeap minHeap = {{0}, 0}; // Pour les moins consommatrices

        // Collecter les stations dans les tas
        collectTopAndBottomNodes(root, &minHeap, &maxHeap);
        NodeHeap mergedHeap = mergeHeaps(&minHeap, &maxHeap);
        sortHeapBySurplus(&mergedHeap);

        // Ouvrir le fichier CSV pour écrire
        FILE* file = fopen("../tests/lv_all_minmax.csv", "w");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier CSV");
            return 1;
        }
        // Ecrire le résultats dans un fichier csv
        writeHeapToCSV("../tests/lv_all_minmax.csv", &mergedHeap);
    }   

    // Libérer la mémoire allouée à l'arbre (non inclus ici)
    return 0;
}
