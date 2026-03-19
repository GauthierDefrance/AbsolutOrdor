/**
 * @file liste_tq.c
 * @brief Implémentation des primitives de gestion de Liste Tête-Queue.
 *
 * Ce module permet de gérer des listes chaînées avec un accès en temps constant O(1)
 * à la fois sur le premier et le dernier élément, ce qui est crucial pour
 * l'implémentation performante de files d'attente.
 */
#include "liste_tq.h"
#include <stdlib.h>
#include <stdio.h>


/*******************************************
    Allocation mémoire
********************************************/


/**
 * @brief Alloue et sécurise la structure de contrôle ListeTQ.
 *
 * Crée l'enveloppe qui contient les pointeurs de début et de fin.
 *
 * @return ListeTQ Le pointeur vers la structure initialisée, ou NULL en cas d'échec.
 */
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


/**
 * @brief Libère uniquement le conteneur ListeTQ.
 *
 * Cette fonction ne libère pas les maillons de la liste. Elle est utile si
 * la liste a déjà été vidée ou si les maillons sont gérés ailleurs.
 *
 * @param ltq Double pointeur vers la structure à libérer (mis à NULL après libération).
 */
void libMemLTQ(ListeTQ *ltq) {
    if (!ltq || !*ltq) return;
    free(*ltq);
    *ltq = NULL;
}


/**
 * @brief Nettoyage complet, récursif et sécurisé de la liste.
 *
 * Parcourt chaque maillon, libère la donnée utilisateur via le callback freeData,
 * puis libère le maillon lui-même avant de libérer le conteneur final.
 *
 * @param listeTQ  La liste à détruire.
 * @param freeData Fonction de libération de la donnée (passer NULL pour ne pas libérer data).
 */
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


/**
 * @brief Remet à zéro les pointeurs d'une liste existante.
 *
 * @param listeTQ La structure à réinitialiser.
 */
void initLTQ(ListeTQ listeTQ) {
    if (!listeTQ) return;
    listeTQ->tete = NULL;
    listeTQ->queue = NULL;
}


/*******************************************
    Primitives
********************************************/


/**
 * @brief Accesseur vers le premier maillon.
 *
 * @return Liste Le pointeur vers la tête de liste.
 */
Liste teteLTQ(ListeTQ listeTQ) {
    return listeTQ ? listeTQ->tete : NULL;
}


/**
 * @brief Accesseur vers le dernier maillon.
 *
 * @return Liste Le pointeur vers la queue de liste.
 */
Liste queueLTQ(ListeTQ listeTQ) {
    return listeTQ ? listeTQ->queue : NULL;
}


/*******************************************
    Insertion
********************************************/


/**
 * @brief Insère un nouvel élément au début de la liste (O(1)).
 *
 * Gère la mise à jour automatique du pointeur de queue si la liste était vide.
 *
 * @param listeTQ La liste cible.
 * @param data    La donnée à stocker dans le nouveau maillon.
 */
void inserTeteLTQ(ListeTQ listeTQ, void *data) {
    if (!listeTQ) return;

    Liste cellule = createListe(data);
    if (!cellule) return;

    cellule->suivant = listeTQ->tete;
    listeTQ->tete = cellule;

    if (listeTQ->queue == NULL)
        listeTQ->queue = cellule;
}


/**
 * @brief Insertion en queue optimisée (O(1)).
 *
 * Utilise le pointeur de queue pour ajouter l'élément sans parcourir la liste.
 * C'est l'opération fondamentale pour les algorithmes d'ordonnancement (Ready Queue).
 *
 * @param listeTQ La liste cible.
 * @param data    La donnée à stocker.
 */
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


/**
 * @brief Supprime le premier maillon de la liste (O(1)).
 *
 * Met à jour le pointeur de tête et assure la cohérence du pointeur de queue
 * si la liste devient vide.
 *
 * @param listeTQ  La liste cible.
 * @param freeData Callback pour libérer la donnée du maillon supprimé.
 */
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


/**
 * @brief Supprime le dernier maillon de la liste (O(n)).
 *
 * Nécessite un parcours de la liste pour trouver l'avant-dernier élément afin
 * de mettre à jour le pointeur de queue.
 *
 * @param listeTQ  La liste cible.
 * @param freeData Callback de libération de la donnée.
 */
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
 * @brief Extraction chirurgicale d'un noeud spécifique.
 * Cette fonction permet de retirer un processus du milieu d'une file (par exemple
 * pour un algorithme de priorité ou SJF). Elle gère les transitions de tête,
 * de queue et le recollage des maillons adjacents.
 *
 * @param ltq      La liste dont on veut extraire le maillon.
 * @param cible    Le maillon exact à retirer.
 * @param freeData Callback pour libérer la donnée (passer NULL pour garder la donnée).
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