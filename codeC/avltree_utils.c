#include <stdio.h>
#include <stdlib.h>
#include "avltree_utils.h"
#include "avltree_usage.h"

#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

// Fonction pour créer un noeud représentant une station avec ces informations
AVLNode* createNode(int station_id, long capacity) {
    AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
    if (newNode == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    newNode->station_id = station_id;
    newNode->capacity = capacity;
    newNode->load = 0; // Initialement aucune consommation
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1; // Un nouveau nœud a une hauteur de 1
    return newNode;
}

// Fonction pour récupérer la hauteur d'un noeud
int getHeight(AVLNode* node) {
    return (node == NULL) ? 0 : node->height;
}

// Fonction pour vérifier l'équilibre entre deux noeuds
int getBalance(AVLNode* node) {
    return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right);
}

// Fonction pour appliquer la rotation par la droite
AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    // Rotation
    x->right = y;
    y->left = T2;

    // Mise à jour des hauteurs
    y->height = 1 + MAX(getHeight(y->left), getHeight(y->right));
    x->height = 1 + MAX(getHeight(x->left), getHeight(x->right));

    return x;
}

// Fonction pour appliquer la rotation par la gauche
AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    // Rotation
    y->left = x;
    x->right = T2;

    // Mise à jour des hauteurs
    x->height = 1 + MAX(getHeight(x->left), getHeight(x->right));
    y->height = 1 + MAX(getHeight(y->left), getHeight(y->right));

    return y;
}

// Fonction pour insérer un nouveau noeud à l'arbre AVL et équilibrer ce dernier
AVLNode* insertNode(AVLNode* node, int station_id, long capacity) {
    // Insertion standard dans un ABR
    if (node == NULL) return createNode(station_id, capacity);

    if (station_id < node->station_id)
        node->left = insertNode(node->left, station_id, capacity);
    else if (station_id > node->station_id)
        node->right = insertNode(node->right, station_id, capacity);
    else
        return node; // Pas de duplication de station_id

    // Mise à jour de la hauteur
    node->height = 1 + MAX(getHeight(node->left), getHeight(node->right));

    // Vérification de l’équilibre
    int balance = getBalance(node);

    // Cas 1 : Gauche-Gauche
    if (balance > 1 && station_id < node->left->station_id)
        return rotateRight(node);

    // Cas 2 : Droite-Droite
    if (balance < -1 && station_id > node->right->station_id)
        return rotateLeft(node);

    // Cas 3 : Gauche-Droite
    if (balance > 1 && station_id > node->left->station_id) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Cas 4 : Droite-Gauche
    if (balance < -1 && station_id < node->right->station_id) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Fonction pour vérifier l'équilibre d'un arbre AVL
int isBalanced(AVLNode* root) {
    // Vérifie si l'arbre est vide, dans ce cas il est équilibré
    if (root == NULL) {
        return 1;
    }

    // Vérifie la hauteur des sous-arbres gauche et droit
    int balance_factor = getBalance(root);

    // Si le facteur d'équilibre est entre -1 et 1, l'arbre est équilibré
    if (balance_factor < -1 || balance_factor > 1) {
        return 0;
    }

    // Vérifie récursivement les sous-arbres
    return isBalanced(root->left) && isBalanced(root->right);
}
