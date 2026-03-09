/**
 * @file liste_tq.c
 * @brief Implémentation des fonctions de gestion de la liste Tête-Queue (ListeTQ).
 */


#include "liste_tq.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour une structure ListeTQ.
 * @note  Alloue uniquement la structure ListeTQ.
 *        Les champs tete et queue doivent être initialisés
 *        via initLTQ() après l’allocation.
 * @return ListeTQ - Pointeur vers la ListeTQ allouée, NULL en cas d’échec.
 */
ListeTQ allocMemLTQ() {
    ListeTQ lp = (ListeTQ) malloc(sizeof(*lp));
    if ( lp == NULL ) { fprintf(stderr, "Erreur : echec de l'allocation memoire\n"); }
    return lp;
}



/*******************************************
    Fonction libération mémoire
********************************************/
/**
 * @brief Libère la mémoire de la structure ListeTQ et met le pointeur à NULL.
 * @note  Ne libère pas les cellules de la liste — utiliser destroyLTQ pour cela.
 * @param ltq - Adresse du pointeur vers la ListeTQ à libérer.
 * @warning Le pointeur ltq doit être valide et non NULL.
 */
void libMemLTQ(ListeTQ *ltq) {
    free(*ltq);
    *ltq = NULL;
}



/******************************************
    Fonction de destruction
******************************************/
/**
 * @brief Détruit complètement une ListeTQ.
 * @note  Libère toutes les cellules de la liste ainsi que la structure ListeTQ.
 *        Utilise suppTeteLTQ pour supprimer les cellules en cascade.
 * @param listeTQ - ListeTQ à détruire (pointeur vers la structure).
 */
void destroyLTQ(ListeTQ listeTQ) {
    if ( listeTQ == NULL ) return;
    while ( listeTQ->tete != NULL ) {
        suppTeteLTQ(listeTQ);
    }
    free(listeTQ);
}



/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise les champs tete et queue d'une ListeTQ à NULL.
 * @param listeTQ - ListeTQ à initialiser.
 * @pre listeTQ != NULL
 */
void initLTQ(ListeTQ listeTQ) {
    listeTQ->tete = NULL;
    listeTQ->queue = NULL;
}



/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Retourne la tête de la ListeTQ.
 * @param listeTQ - ListeTQ.
 * @return Liste - Pointeur vers la cellule de tête, NULL si la liste est vide.
 */
Liste teteLTQ(ListeTQ listeTQ) {
    return listeTQ->tete;
}


/**
 * @brief Retourne la queue de la ListeTQ.
 * @note  Complexité O(1) — préférer cette fonction à un parcours complet.
 * @param listeTQ - ListeTQ.
 * @return Liste - Pointeur vers la cellule de queue, NULL si la liste est vide.
 */
Liste queueLTQ(ListeTQ listeTQ) {
    return listeTQ->queue;
}



/******************************************
    Fonction d'insertion
*******************************************/
/**
 * @brief Insère un élément en tête de la ListeTQ.
 * @note  Met à jour les pointeurs tete et queue si la liste était vide.
 * @param listeTQ - ListeTQ.
 * @param elem - Élément à insérer.
 */
void inserTeteLTQ(ListeTQ listeTQ, int elem) {
    //Opération impossible
    if (listeTQ == NULL) return;

    //On a 0 éléments
    if (teteLTQ(listeTQ) == NULL ) {
        Liste liste = createListe(elem);
        listeTQ->tete = liste;
        listeTQ->queue = liste;
        return;
    }

    //On a 1 élements ou plus
    inserTete(&listeTQ->tete, elem);
}


/**
 * @brief Insère un élément en queue de la ListeTQ.
 * @note  Complexité O(1) grâce au pointeur queue.
 *        Met à jour les pointeurs tete et queue si la liste était vide.
 * @param listeTQ - ListeTQ.
 * @param elem - Élément à insérer.
 */
void inserQueueLTQ(ListeTQ listeTQ, int elem) {
    if (listeTQ == NULL) return; //OP Impossible

    Liste nouvelle = createListe(elem);
    if (nouvelle == NULL) return;

    if (listeTQ->tete == NULL) {
        // On a 0 éléments
        listeTQ->tete = nouvelle;
        listeTQ->queue = nouvelle;
    }

    else {
        //Cas classique, 1 élem ou plus
        listeTQ->queue->suivant = nouvelle;
        listeTQ->queue = nouvelle;
    }
}



/******************************************
    Fonction de suppression
*******************************************/
/**
 * @brief Supprime la cellule en tête de la ListeTQ et libère sa mémoire.
 * @note  Met le pointeur queue à NULL si la liste devient vide.
 * @param listeTQ - ListeTQ.
 */
void suppTeteLTQ(ListeTQ listeTQ) {
    if (listeTQ == NULL || estVideListe(listeTQ->tete)) return;

    suppTete(&(listeTQ->tete)); // suppTete effectue le décalage

    if (listeTQ->tete == NULL) {
        listeTQ->queue = NULL;
    }
}


/**
 * @brief Supprime la cellule en queue de la ListeTQ et libère sa mémoire.
 * @note  Met à jour le pointeur queue vers l’avant-dernier élément.
 *        Complexité O(N) car un parcours est nécessaire.
 * @param listeTQ - ListeTQ.
 */
void suppQueueLTQ(ListeTQ listeTQ) {
    if (listeTQ == NULL || estVideListe(listeTQ->tete)) return;

    // 1 élément
    if (listeTQ->tete == listeTQ->queue) {
        free(listeTQ->tete);
        listeTQ->tete = NULL;
        listeTQ->queue = NULL;
        return;
    }

    //Localisation de l'avant dernier element
    Liste courant = listeTQ->tete;
    while (courant->suivant != listeTQ->queue) {
        courant = courant->suivant;
    }

    //Libération mémoire du dernier elem
    free(listeTQ->queue);
    courant->suivant = NULL;
    listeTQ->queue = courant;
}