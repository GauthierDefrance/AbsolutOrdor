#pragma once
#include "liste.h"

/**
 * Liste Tête-Queue générique.
 * Contient des cellules de type Liste (déjà générique).
 */
typedef struct ListeTQ_Cellule {
    Liste tete;
    Liste queue;
} *ListeTQ;


/*******************************************
    Allocation mémoire
********************************************/
ListeTQ allocMemLTQ();


/*******************************************
    Libération mémoire
********************************************/

/**
 * @brief Libère uniquement la structure ListeTQ (pas les cellules).
 */
void libMemLTQ(ListeTQ *ltq);

/**
 * @brief Détruit complètement la liste TQ + toutes ses cellules.
 * @param listeTQ - structure ListeTQ
 * @param freeData - callback pour libérer data dans chaque cellule (optionnel)
 */
void destroyLTQ(ListeTQ listeTQ, void (*freeData)(void *));


/*******************************************
    Initialisation
********************************************/
void initLTQ(ListeTQ listeTQ);


/*******************************************
    Fonctions primitives
********************************************/
Liste teteLTQ(ListeTQ listeTQ);
Liste queueLTQ(ListeTQ listeTQ);


/*******************************************
    Insertion
********************************************/

/**
 * @brief Insère un élément en tête (O(1)).
 */
void inserTeteLTQ(ListeTQ listeTQ, void *data);

/**
 * @brief Insère un élément en queue (O(1)).
 */
void inserQueueLTQ(ListeTQ listeTQ, void *data);


/*******************************************
    Suppression
********************************************/
void suppTeteLTQ(ListeTQ listeTQ, void (*freeData)(void *));
void suppQueueLTQ(ListeTQ listeTQ, void (*freeData)(void *));
