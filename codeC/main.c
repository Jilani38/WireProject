#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree_usage.h"
#include "avltree_utils.h"
#include "utils.h"



int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <station_type> <consumer_type>\n", argv[0]);
        return 1;
    }

    char* station_type = argv[1];
    char* consumer_type = argv[2];

    char filename[256];
    snprintf(filename, sizeof(filename), "../tests/%s_%s.csv", station_type, consumer_type);

    int is_lv_all = strcmp(consumer_type, "all") == 0;

    // Create AVL tree
    AVLNode* root = NULL;

    // Insert stations
    root = readAndInsertStations("../tmp/stations_data.csv", root);

    // Read and add consumers to the AVL tree
    readAndAddConsumers("../tmp/consumers_data.csv", root);

    // Initialize dynamic array to collect data
    NodeCollection* collection = initNodeCollection(100); // Initial capacity of 100

    // Collect nodes from AVL into a collection
    collectNodesInOrder(root, collection);

    // Sort nodes by increasing capacity
    qsort(collection->nodes, collection->size, sizeof(AVLNode*), compareByCapacity);

    // Open and initialize output file
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening CSV file");
        freeAVLTree(root);  // Free the tree before exiting
        free(collection->nodes);
        free(collection);
        return 1;
    }

    // Write headers
    toUpperCase(station_type);
    toUpperCase(consumer_type);
    fprintf(file, "Station %s,Capacity,Load (%s)\n", station_type, consumer_type);

    // Write sorted results to CSV
    writeCollectionToCSV(collection, file);

    //Changing filename 
    char *new_start = filename + 2;
    strcpy(filename, new_start);
    printf("Sorted results have been written to %s.\n", filename);

    // Close file
    fclose(file);

    // For lv all case
    if (is_lv_all) {
        // Initialize collections for min and max
        NodeCollection* maxCollection = initNodeCollection(10); // For highest consumers
        NodeCollection* minCollection = initNodeCollection(10); // For lowest consumers

        // Collect stations into collections
        collectTopAndBottomNodes(root, minCollection, maxCollection);;
        

        // Merge both collections
        NodeCollection* mergedCollection = initNodeCollection(20); // Assuming 20 for both min and max combined
        for (int i = 0; i < minCollection->size; ++i) {
            addNodeToCollection(mergedCollection, minCollection->nodes[i]);
        }
        for (int i = 0; i < maxCollection->size; ++i) {
            addNodeToCollection(mergedCollection, maxCollection->nodes[i]);
        }
        
        //Sort collections by surplus
        sortCollectionBySurplus(mergedCollection);
        

        // Open CSV file for writing
        FILE* lv_all_file = fopen("../tests/lv_all_minmax.csv", "w");
        if (lv_all_file == NULL) {
            perror("Error opening CSV file for lv_all_minmax");
            // Free all collections and exit
            freeAVLTree(root);
            free(collection->nodes);
            free(collection);
            free(maxCollection->nodes);
            free(maxCollection);
            free(minCollection->nodes);
            free(minCollection);
            free(mergedCollection->nodes);
            free(mergedCollection);
            return 1;
        }

        // Write headers
        fprintf(lv_all_file, "Station %s,Capacity,Load (%s)\n", station_type, consumer_type);

        // Write results to CSV
        writeCollectionToCSV(mergedCollection, lv_all_file);

        printf("Filtered results have been written to /tests/lv_all_minmax.\n");

        // Close file and free memory
        fclose(lv_all_file);
        free(maxCollection->nodes);
        free(maxCollection);
        free(minCollection->nodes);
        free(minCollection);
        free(mergedCollection->nodes);
        free(mergedCollection);
    }

    // Free all remaining dynamically allocated memory
    free(collection->nodes);
    free(collection);
    freeAVLTree(root);

    return 0;
}
