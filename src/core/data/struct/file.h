#pragma once
#include "data/struct/liste_tq.h"
#include <stdbool.h>
/**
 * @file file.h
 * @brief Interface d'une file (Queue) générique basée sur une Liste Tête-Queue.
 *
 * @details Cette structure implémente une logique FIFO (First-In, First-Out).
 * Elle est conçue pour être performante grâce à l'utilisation d'une ListeTQ
 * qui permet des ajouts en queue et des retraits en tête en temps constant O(1).
 */


/**
 * @struct File_Cellule
 * @brief Structure de contrôle de la File.
 *
 * Pointeur vers la structure ListeTQ interne qui gère le chaînage des éléments.
 */
typedef struct File_Cellule {
    ListeTQ ltq;
} *File;


/*******************************************
    Allocation / Initialisation
********************************************/


/**
 * @brief Alloue la mémoire pour une nouvelle File.
 *
 * @return File Le pointeur vers la file allouée et initialisée,
 * ou NULL en cas d'échec d'allocation.
 */
File allocFile();

/**
 * @brief Initialise une file existante à un état vide.
 *
 * @param f La file à initialiser.
 */
void initFile(File f);


/*******************************************
    Opérations principales
********************************************/


/**
 * @brief Ajoute un élément à la fin de la file (Enfiler / Enqueue).
 *
 * @param f    La file cible.
 * @param data Pointeur générique vers la donnée à stocker.
 */
void enfilerFile(File f, void *data);

/**
 * @brief Retire et retourne l'élément situé en tête de la file (Défiler / Dequeue).
 *
 * @warning Cette fonction retire le maillon de la file mais **ne libère pas** * la mémoire de la donnée 'data'. L'appelant devient propriétaire de ce pointeur.
 *
 * @param f La file cible.
 * @return void* Le pointeur vers la donnée récupérée, ou NULL si la file est vide.
 */
void *defilerFile(File f);

/**
 * @brief Accède à l'élément en tête de file sans le retirer (Peek).
 *
 * @param f La file à consulter.
 * @return void* Le pointeur vers la donnée en tête, ou NULL si la file est vide.
 */
void *teteFile(File f);

/**
 * @brief Vérifie si la file est vide.
 *
 * @param f La file à tester.
 * @return true  Si la file ne contient aucun élément.
 * @return false Si la file contient au moins un maillon.
 */
bool estVideFile(File f);

/**
 * @brief Calcule le nombre d'éléments présents dans la file.
 *
 * @param f La file cible.
 * @return int Le nombre de maillons (0 si la file est vide ou NULL).
 *
 * @note Cette opération a une complexité temporelle de O(n).
 */
int tailleFile(File f);

/*******************************************
    Destruction
********************************************/

/**
 * @brief Détruit la structure File et libère toute la mémoire associée.
 *
 * @details Cette fonction libère le conteneur interne, tous les maillons et,
 * si un callback est fourni, les données utilisateur.
 *
 * @param f La file à détruire.
 * @param freeData Fonction de rappel optionnelle pour libérer les données
 * stockées (void *). Passer NULL pour ne libérer que la structure.
 */
void destroyFile(File f, void (*freeData)(void *));
