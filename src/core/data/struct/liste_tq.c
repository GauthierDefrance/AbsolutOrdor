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

/**
 * Supprime un noeud spécifique d'une liste sans casser la structure globale.
 * @param ltq : La liste dont on veut supprimer un maillon
 * @param cible : Le noeud (cellule) à retirer
 * @param freeData : Fonction de libération de la donnée (passer NULL si non utilisé)
 */
void supprimerNoeudLTQ(ListeTQ ltq, Liste cible, void (*freeData)(void *)) {
    if (!ltq || !cible) return;

    // --- 1. Cas : C'est la tête de liste ---
    if (ltq->tete == cible) {
        ltq->tete = cible->suivant;
        if (ltq->tete == NULL) ltq->queue = NULL;
    } 
    // --- 2. Cas : C'est ailleurs ou à la queue ---
    else {
        Liste courant = ltq->tete;
        // On cherche le prédécesseur du noeud cible
        while (courant != NULL && courant->suivant != cible) {
            courant = courant->suivant;
        }

        // Si on a trouvé le prédécesseur
        if (courant != NULL) {
            courant->suivant = cible->suivant;
            // Si c'était la queue, on met à jour le pointeur queue
            if (cible == ltq->queue) {
                ltq->queue = courant;
            }
        } else {
            // Le noeud cible n'appartient pas à la liste
            return;
        }
    }

    // --- 3. Libération de la mémoire ---
    // On libère la donnée si le callback est fourni
    if (freeData && cible->data) {
        freeData(cible->data);
    }
    // On libère la cellule elle-même
    free(cible);
}