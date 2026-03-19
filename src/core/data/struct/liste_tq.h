#pragma once
#include "liste.h"
/**
 * @file liste_tq.h
 * @brief Structure de liste chaînée avec pointeurs de tête et de queue.
 *
 * Cette structure encapsule une liste simple pour permettre des insertions
 * rapides aux deux extrémités, ce qui est idéal pour l'implémentation de files (FIFO).
 */


/*******************************************
    Structure de données
********************************************/


/**
 * @struct ListeTQ_Cellule
 * @brief Contrôleur de liste contenant les accès directs au début et à la fin.
 */
typedef struct ListeTQ_Cellule {
    Liste tete;
    Liste queue;
} *ListeTQ;


/*******************************************
    Allocation mémoire
********************************************/


/**
 * @brief Alloue la structure de contrôle ListeTQ.
 *
 * @return ListeTQ Un pointeur vers la structure initialisée à NULL, ou NULL si échec.
 */
ListeTQ allocMemLTQ();


/*******************************************
    Libération mémoire
********************************************/


/**
 * @brief Libère uniquement la structure de contrôle (le conteneur).
 *
 * Attention : les cellules (maillons) ne sont pas libérées par cette fonction.
 *
 * @param ltq Pointeur vers le pointeur de la liste à libérer.
 */
void libMemLTQ(ListeTQ *ltq);

/**
 * @brief Détruit intégralement la liste (Conteneur + Cellules + Données).
 *
 * @param listeTQ La liste à détruire.
 * @param freeData Fonction de rappel (callback) pour libérer le contenu de chaque cellule.
 */
void destroyLTQ(ListeTQ listeTQ, void (*freeData)(void *));


/*******************************************
    Initialisation
********************************************/


/** @brief Réinitialise une liste existante (met les pointeurs à NULL). */
void initLTQ(ListeTQ listeTQ);


/*******************************************
    Fonctions primitives
********************************************/


/** @return La cellule de tête (premier élément). */
Liste teteLTQ(ListeTQ listeTQ);

/** @return La cellule de queue (dernier élément). */
Liste queueLTQ(ListeTQ listeTQ);


/*******************************************
    Insertion
********************************************/


/** @brief Ajoute une donnée au début de la liste. */
void inserTeteLTQ(ListeTQ listeTQ, void *data);

/**
 * @brief Ajoute une donnée à la fin de la liste.
 * Grâce au pointeur de queue, cette opération est instantanée.
 */
void inserQueueLTQ(ListeTQ listeTQ, void *data);


/*******************************************
    Suppression
********************************************/


/** @brief Supprime le premier élément de la liste. */
void suppTeteLTQ(ListeTQ listeTQ, void (*freeData)(void *));

/** @brief Supprime le dernier élément de la liste (O(n)). */
void suppQueueLTQ(ListeTQ listeTQ, void (*freeData)(void *));

/**
 * @brief Retire un noeud spécifique de la liste sans rompre le chaînage.
 *
 * @param ltq    La liste parente.
 * @param cible  Le noeud exact à extraire.
 * @param freeData Optionnel : libère la donnée contenue dans le noeud.
 */
void supprimerNoeudLTQ(ListeTQ ltq, Liste cible, void (*freeData)(void *));