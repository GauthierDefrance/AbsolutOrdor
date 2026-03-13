/**
 * @file liste.c
 * @brief Implémentation des fonctions de gestion de la liste générique chaînée.
 */

#include "liste.h"


/*******************************************
    Fonction allocation mémoire
********************************************/

Liste allocListe() {
    Liste cellule = (Liste) malloc(sizeof(struct Cellule));
    if (cellule == NULL) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire d'une cellule\n");
        return NULL;
    }
    cellule->data = NULL;
    cellule->suivant = NULL;
    return cellule;
}


/******************************************
    Fonction de destruction
******************************************/

void suppTete(Liste *l, void (*freeData)(void *)) {
    if (l == NULL || *l == NULL) return;

    Liste tmp = *l;
    *l = tmp->suivant;

    if (freeData && tmp->data)
        freeData(tmp->data);

    free(tmp);
}

void suppQueue(Liste *l, void (*freeData)(void *)) {
    if (l == NULL || *l == NULL) return;

    if ((*l)->suivant == NULL) {
        if (freeData && (*l)->data)
            freeData((*l)->data);
        free(*l);
        *l = NULL;
        return;
    }

    Liste courant = *l;
    while (courant->suivant->suivant != NULL) {
        courant = courant->suivant;
    }

    Liste dernier = courant->suivant;
    if (freeData && dernier->data)
        freeData(dernier->data);
    free(dernier);
    courant->suivant = NULL;
}

void destroyListe(Liste liste, void (*freeData)(void *)) {
    while (liste != NULL) {
        Liste tmp = liste;
        liste = liste->suivant;

        if (freeData && tmp->data)
            freeData(tmp->data);

        free(tmp);
    }
}


/******************************************
    Fonction d'initialisation
******************************************/

void initListe(Liste cellule) {
    if (cellule == NULL) return;
    cellule->suivant = NULL;
}


/******************************************
    Fonction de création
******************************************/

Liste createListe(void *data) {
    Liste cellule = allocListe();
    if (!cellule) return NULL;
    cellule->data = data;
    return cellule;
}

void setCelluleData(Liste cellule, void *data) {
    if (cellule == NULL) return;
    cellule->data = data;
}


/******************************************
    Fonction primitive
******************************************/

Liste suivantListe(Liste cellule) {
    if (cellule == NULL) return NULL;
    return cellule->suivant;
}

Liste queueListe(Liste l) {
    if (l == NULL) return NULL;
    while (l->suivant != NULL) {
        l = l->suivant;
    }
    return l;
}

bool estVideListe(Liste l) {
    return l == NULL;
}

void *donneeListe(Liste l) {
    if (l == NULL) return NULL;
    return l->data;
}


int tailleListe(Liste l) {
    if (l == NULL) return 0;
    int compteur = 1;
    while (l->suivant != NULL) {
        l = l->suivant;
        compteur++;
    }
    return compteur;
}

/******************************************
    Fonction d'insertion
******************************************/

void inserTete(Liste *l, void *data) {
    if (l == NULL) return;

    Liste cellule = createListe(data);
    if (cellule == NULL) return;

    cellule->suivant = *l;
    *l = cellule;
}

void inserQueue(Liste *l, void *data) {
    if (l == NULL) return;

    Liste cellule = createListe(data);
    if (cellule == NULL) return;

    if (*l == NULL) {
        *l = cellule;
        return;
    }

    Liste queue = queueListe(*l);
    queue->suivant = cellule;
}


/******************************************
    Fonction de parcours / affichage
******************************************/

void printListe(Liste l, void (*printData)(void *)) {
    if (estVideListe(l)) {
        printf("[Liste vide]");
        return;
    }

    Liste courant = l;

    while (courant != NULL) {
        if (printData)
            printData(courant->data);
        else
            printf("[data=%p]", courant->data);

        if (courant->suivant != NULL)
            printf(" -> ");

        courant = courant->suivant;
    }

    printf(" -> NULL");
}
