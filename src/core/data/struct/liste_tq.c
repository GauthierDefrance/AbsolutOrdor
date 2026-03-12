#include "liste_tq.h"
#include <stdlib.h>
#include <stdio.h>


/*******************************************
    Allocation mémoire
********************************************/
ListeTQ allocMemLTQ() {
    ListeTQ ltq = malloc(sizeof(struct ListeTQ_Cellule));
    if (!ltq) {
        fprintf(stderr, "Erreur : echec allocation ListeTQ\n");
        return NULL;
    }
    ltq->tete = NULL;
    ltq->queue = NULL;
    return ltq;
}


/*******************************************
    Libération mémoire
********************************************/
void libMemLTQ(ListeTQ *ltq) {
    if (!ltq || !*ltq) return;
    free(*ltq);
    *ltq = NULL;
}

void destroyLTQ(ListeTQ listeTQ, void (*freeData)(void *)) {
    if (!listeTQ) return;

    Liste courant = listeTQ->tete;
    while (courant != NULL) {
        Liste tmp = courant;
        courant = courant->suivant;

        if (freeData && tmp->data)
            freeData(tmp->data);

        free(tmp);
    }

    free(listeTQ);
}


/*******************************************
    Initialisation
********************************************/
void initLTQ(ListeTQ listeTQ) {
    if (!listeTQ) return;
    listeTQ->tete = NULL;
    listeTQ->queue = NULL;
}


/*******************************************
    Primitives
********************************************/
Liste teteLTQ(ListeTQ listeTQ) {
    return listeTQ ? listeTQ->tete : NULL;
}

Liste queueLTQ(ListeTQ listeTQ) {
    return listeTQ ? listeTQ->queue : NULL;
}


/*******************************************
    Insertion
********************************************/
void inserTeteLTQ(ListeTQ listeTQ, void *data) {
    if (!listeTQ) return;

    Liste cellule = createListe(data);
    if (!cellule) return;

    cellule->suivant = listeTQ->tete;
    listeTQ->tete = cellule;

    if (listeTQ->queue == NULL)
        listeTQ->queue = cellule;
}

void inserQueueLTQ(ListeTQ listeTQ, void *data) {
    if (!listeTQ) return;

    Liste cellule = createListe(data);
    if (!cellule) return;

    if (listeTQ->tete == NULL) {
        listeTQ->tete = cellule;
        listeTQ->queue = cellule;
        return;
    }

    listeTQ->queue->suivant = cellule;
    listeTQ->queue = cellule;
}


/*******************************************
    Suppression
********************************************/
void suppTeteLTQ(ListeTQ listeTQ, void (*freeData)(void *)) {
    if (!listeTQ || !listeTQ->tete) return;

    Liste tmp = listeTQ->tete;
    listeTQ->tete = tmp->suivant;

    if (freeData && tmp->data)
        freeData(tmp->data);

    free(tmp);

    if (listeTQ->tete == NULL)
        listeTQ->queue = NULL;
}

void suppQueueLTQ(ListeTQ listeTQ, void (*freeData)(void *)) {
    if (!listeTQ || !listeTQ->tete) return;

    // 1 élément
    if (listeTQ->tete == listeTQ->queue) {
        if (freeData && listeTQ->tete->data)
            freeData(listeTQ->tete->data);

        free(listeTQ->tete);
        listeTQ->tete = NULL;
        listeTQ->queue = NULL;
        return;
    }

    // Plusieurs éléments
    Liste courant = listeTQ->tete;
    while (courant->suivant != listeTQ->queue)
        courant = courant->suivant;

    if (freeData && listeTQ->queue->data)
        freeData(listeTQ->queue->data);

    free(listeTQ->queue);
    courant->suivant = NULL;
    listeTQ->queue = courant;
}
