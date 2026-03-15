#pragma once
#include "data/struct/liste_tq.h"
#include <stdbool.h>

/**
 * File générique basée sur ListeTQ.
 * FIFO : First-In First-Out
 */
typedef struct File_Cellule {
    ListeTQ ltq;
} *File;


/*******************************************
    Allocation / Initialisation
********************************************/
File allocFile();
void initFile(File f);


/*******************************************
    Opérations principales
********************************************/

/**
 * @brief Ajoute un élément en queue (enqueue).
 */
void enfilerFile(File f, void *data);

/**
 * WARNING : Ne fait pas de libération mémoire
 * @brief Retire et retourne l’élément en tête (dequeue).
 * @return data ou NULL si vide.
 */
void *defilerFile(File f);

/**
 * @brief Retourne l’élément en tête sans le retirer.
 */
void *teteFile(File f);

/**
 * @brief Indique si la file est vide.
 */
bool estVideFile(File f);

/**
 * @brief Renvoie la taille de la file.
 */
int tailleFile(File f);

/*******************************************
    Destruction
********************************************/

/**
 * @brief Détruit la file et toutes ses données.
 * @param freeData - callback pour libérer data (optionnel)
 */
void destroyFile(File f, void (*freeData)(void *));
