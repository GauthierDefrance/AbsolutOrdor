/**
* @file liste_tq.c
 * @brief Implémentation des fonctions de gestion de la liste Tête-Queue (ListeTQ).
 */


#include "liste_tq.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @file liste_tq.c
 * @brief Implémentation des fonctions de gestion de la liste Tête-Queue (ListeTQ).
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
 */
void libMemLTQ(ListeTQ *ltq) {
    free(*ltq);
    *ltq = NULL;
}



/******************************************
    Fonction de destruction
******************************************/
/**
 * @brief Détruit complètement une ListeTQ en libérant toutes ses cellules
 *        ainsi que la structure ListeTQ elle-même.
 * @note  Utilise suppTeteLTQ pour libérer chaque cellule en cascade.
 * @param listeTQ - Pointeur vers la ListeTQ à détruire.
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
 * @param listeTQ - Pointeur vers la ListeTQ à initialiser.
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
 * @param listeTQ - Pointeur vers la ListeTQ.
 * @return ListeGenerique - Pointeur vers la cellule de tête, NULL si vide.
 */
ListeGenerique teteLTQ(ListeTQ listeTQ) {
    return listeTQ->tete;
}


/**
 * @brief Retourne la queue de la ListeTQ.
 * @note  Complexité O(1) — préférer cette fonction à queueListe() qui est O(N).
 * @param listeTQ - Pointeur vers la ListeTQ.
 * @return ListeGenerique - Pointeur vers la cellule de queue, NULL si vide.
 */
ListeGenerique queueLTQ(ListeTQ listeTQ) {
    return listeTQ->queue;
}



/******************************************
    Fonction d'insertion
*******************************************/
/**
 * @brief Insère une donnée en tête de la ListeTQ.
 * @note  Met à jour tete et queue si la liste était vide.
 * @param listeTQ - Pointeur vers la ListeTQ.
 * @param data    - Pointeur vers la donnée à insérer.
 */
void inserTeteLTQ(ListeTQ listeTQ, Data *data) {
    //Opération impossible
    if ( data == NULL  || listeTQ == NULL) return;

    //On a 0 éléments
    if (teteLTQ(listeTQ) == NULL ) {
        ListeGenerique liste = createListe(data);
        listeTQ->tete = liste;
        listeTQ->queue = liste;
        return;
    }

    //On a 1 élements ou plus
    inserTete(&listeTQ->tete, data);
}


/**
 * @brief Insère une donnée en queue de la ListeTQ.
 * @note  Complexité O(1) grâce au pointeur queue.
 *        Met à jour tete et queue si la liste était vide.
 * @param listeTQ - Pointeur vers la ListeTQ.
 * @param data    - Pointeur vers la donnée à insérer.
 */
void inserQueueLTQ(ListeTQ listeTQ, Data *data) {
    if (data == NULL || listeTQ == NULL) return; //OP Impossible

    ListeGenerique nouvelle = createListe(data);
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
 * @note  Met queue à NULL si la liste devient vide après suppression.
 * @param listeTQ - Pointeur vers la ListeTQ.
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
 * @note  Met à jour le pointeur queue vers l'avant-dernier élément.
 *        Complexité O(N) car nécessite un parcours jusqu'à l'avant-dernier élément.
 * @param listeTQ - Pointeur vers la ListeTQ.
 */
void suppQueueLTQ(ListeTQ listeTQ) {
    if (listeTQ == NULL || estVideListe(listeTQ->tete)) return;

    // 1 élément
    if (listeTQ->tete == listeTQ->queue) {
        libereCelluleCourante(listeTQ->tete);
        listeTQ->tete = NULL;
        listeTQ->queue = NULL;
        return;
    }

    //Localisation de l'avant dernier element
    ListeGenerique courant = listeTQ->tete;
    while (courant->suivant != listeTQ->queue) {
        courant = courant->suivant;
    }

    //Libération mémoire du dernier elem
    libereCelluleCourante(listeTQ->queue);
    courant->suivant = NULL;
    listeTQ->queue = courant;
}