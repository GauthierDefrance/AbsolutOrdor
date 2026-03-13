#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************
    Structure de données
********************************************/

/**
 * Liste générique chaînée.
 * Chaque cellule contient un pointeur générique `void *data`.
 * La gestion de l'allocation/libération de `data` est à la charge de l'appelant.
 */
typedef struct Cellule {
    void *data;
    struct Cellule *suivant;
} *Liste;


/*******************************************
    Fonction allocation mémoire
********************************************/

/**
 * @brief Alloue une cellule de liste (data non initialisé, suivant = NULL).
 * @return Liste - pointeur vers la cellule allouée, NULL en cas d'échec.
 */
Liste allocListe();


/******************************************
    Fonction de destruction
*******************************************/

/**
 * @brief Détruit complètement une liste.
 * @param liste - pointeur vers la tête de la liste.
 * @param freeData - fonction optionnelle pour libérer `data` dans chaque cellule
 *                   (si NULL, seul les cellules sont libérées).
 */
void destroyListe(Liste liste, void (*freeData)(void *));

/**
 * @brief Supprime la cellule en tête de la liste.
 * @param l - adresse du pointeur vers la tête de la liste.
 * @param freeData - fonction optionnelle pour libérer `data`.
 */
void suppTete(Liste *l, void (*freeData)(void *));

/**
 * @brief Supprime la cellule en queue de la liste.
 * @param l - adresse du pointeur vers la tête de la liste.
 * @param freeData - fonction optionnelle pour libérer `data`.
 */
void suppQueue(Liste *l, void (*freeData)(void *));


/******************************************
    Fonction d'initialisation
******************************************/

/**
 * @brief Initialise une cellule (suivant = NULL, data laissé tel quel).
 */
void initListe(Liste cellule);

/**
 * @brief Crée une nouvelle cellule et y stocke le pointeur data.
 * @param data - pointeur générique à stocker.
 * @return Liste - cellule créée, NULL en cas d'échec.
 */
Liste createListe(void *data);

/**
 * @brief Modifie la donnée stockée dans une cellule.
 * @param cellule - cellule cible.
 * @param data - nouveau pointeur à stocker.
 */
void setCelluleData(Liste cellule, void *data);


/******************************************
    Fonction primitive
*******************************************/

/**
 * @brief Retourne la cellule suivante.
 */
Liste suivantListe(Liste cellule);

/**
 * @brief Retourne la dernière cellule de la liste.
 * @warning Complexité O(N).
 */
Liste queueListe(Liste l);

/**
 * @brief Indique si la liste est vide.
 */
bool estVideListe(Liste l);

/**
 * @brief Retourne la donnée stockée dans la cellule.
 */
void *donneeListe(Liste l);



int tailleListe(Liste l);

/******************************************
    Fonction d'insertion
*******************************************/

/**
 * @brief Insère une donnée en tête de la liste.
 * @param l - adresse du pointeur vers la tête.
 * @param data - pointeur à stocker.
 */
void inserTete(Liste *l, void *data);

/**
 * @brief Insère une donnée en queue de la liste.
 * @param l - adresse du pointeur vers la tête.
 * @param data - pointeur à stocker.
 */
void inserQueue(Liste *l, void *data);


/******************************************
    Fonction d'affichage console
*******************************************/

/**
 * @brief Parcourt la liste et affiche chaque élément via un callback.
 * @param l - tête de la liste.
 * @param printData - fonction appelée pour afficher `data` de chaque cellule.
 */
void printListe(Liste l, void (*printData)(void *));