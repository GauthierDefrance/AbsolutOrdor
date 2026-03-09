/**
 * @file liste.c
 * @brief Implémentation des fonctions de gestion de la liste générique chaînée.
 */

#include "liste.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour une cellule de Liste.
 * @return Liste - Pointeur vers la cellule allouée, NULL en cas d’échec.
 */
Liste allocListe() {
    Liste cellule = (Liste) malloc(sizeof(struct Cellule));
    if (cellule == NULL) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire d'une cellule\n");
        return NULL;
    }
    return cellule;
}



/*******************************************
    Fonction libération mémoire
********************************************/



/******************************************
    Fonction de destruction
******************************************/
/**
 * @brief Détruit complètement une Liste.
 * @note  Libère toutes les cellules de la liste en cascade à l’aide de suppTete().
 *        Aucun free final n’est nécessaire car chaque cellule est libérée.
 * @param liste - Pointeur vers la tête de la liste à détruire.
 */
void destroyListe(Liste liste) {
    if (liste == NULL) return;
    while (liste != NULL) {
        suppTete(&liste);
    }
}


/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise une cellule de Liste.
 * @note  Met le pointeur suivant à NULL.
 * @param cellule - Pointeur vers la cellule à initialiser.
 * @pre cellule != NULL
 */
void initListe(Liste cellule) {
    if (cellule == NULL) return;
    cellule->suivant = NULL;
}


/******************************************
    Fonction de création
******************************************/
/**
 * @brief Crée une nouvelle cellule et y stocke la donnée fournie.
 * @param elem - Élément à stocker dans la cellule.
 * @return Liste - Pointeur vers la cellule créée, NULL en cas d’échec.
 */
Liste createListe(int elem){
    Liste cellule = allocListe();
    if (!cellule) return NULL;
    initListe(cellule);
    cellule->data = elem;
    return cellule;
}


/**
 * @brief Modifie la donnée stockée dans une cellule.
 * @note  Écrase la valeur précédente.
 * @param cellule - Pointeur vers la cellule à modifier.
 * @param elem - Élément à stocker.
 */
void setCelluleData(Liste cellule, int elem) {
    if (cellule == NULL) return;
    cellule->data = elem;
}


/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Retourne la cellule suivante dans la liste.
 * @param cellule - Pointeur vers la cellule courante.
 * @return Liste - Pointeur vers la cellule suivante, NULL si fin de liste.
 */
Liste suivantListe(Liste cellule) {
    if (cellule == NULL) return NULL;
    return cellule->suivant;
}

/**
 * @brief Retourne la dernière cellule de la liste.
 * @warning Complexité O(N) — préférer une structure avec pointeur queue si possible.
 * @param l - Pointeur vers la tête de la liste.
 * @return Liste - Pointeur vers la dernière cellule.
 */
Liste queueListe(Liste l) {
    while (!estVideListe(suivantListe(l))) {
        l = suivantListe(l);
    }
    return l;
}

/**
 * @brief Indique si la liste est vide.
 * @param l - Pointeur vers la tête de la liste.
 * @return bool - true si la liste est vide, false sinon.
 */
bool estVideListe(Liste l) { return l == NULL; }

/**
 * @brief Retourne la donnée stockée dans une cellule.
 * @param l - Pointeur vers la cellule.
 * @return int - Donnée contenue dans la cellule.
 * @pre l != NULL
 */
int donneeListe(Liste l) { return l->data; }


/******************************************
    Fonction d'insertion
*******************************************/
/**
 * @brief Insère une donnée en tête de la liste.
 * @param l    - Adresse du pointeur vers la tête de la liste.
 * @param elem - Élément à insérer.
 */
void inserTete(Liste *l,  int elem) {
    if (l == NULL) return;
    Liste liste = createListe(elem);
    if (liste == NULL) return;

    liste->suivant = *l;
    *l = liste;
}

/**
 * @brief Insère une donnée en queue de la liste.
 * @warning Complexité O(N) — préférer inserQueueLTQ() qui est O(1).
 * @param l    - Adresse du pointeur vers la tête de la liste.
 * @param elem - Élément à insérer.
 */
void inserQueue(Liste *l, int elem) {
    if (l == NULL) return;
    Liste liste = createListe(elem);
    if (liste == NULL) return;

    if (*l == NULL) {
        *l = liste;
        return;
    }

    Liste queue = queueListe(*l);
    queue->suivant = liste;
}


/******************************************
    Fonction de suppression
*******************************************/
/**
 * @brief Supprime la cellule en tête de la liste et libère sa mémoire.
 * @param l - Adresse du pointeur vers la tête de la liste.
 */
void suppTete(Liste *l) {
    if (estVideListe(*l)) return;
    Liste tmp = *l;
    *l = suivantListe(*l);
    free(tmp);
}

/**
 * @brief Supprime la cellule en queue de la liste et libère sa mémoire.
 * @note  Parcourt la liste jusqu’à l’avant-dernier élément.
 *        Complexité O(N).
 * @param l - Adresse du pointeur vers la tête de la liste.
 */
void suppQueue(Liste *l) {
    if (l == NULL || *l == NULL) return;

    if (suivantListe(*l) == NULL) {
        free(*l);
        *l = NULL;
        return;
    }

    Liste courant = *l;
    while (suivantListe(suivantListe(courant)) != NULL) {
        courant = suivantListe(courant);
    }

    free(suivantListe(courant));
    courant->suivant = NULL;
}


/******************************************
    Fonction de parcours
*******************************************/
/**
 * @brief Affiche le contenu d’une Liste dans la sortie standard.
 * @note  Affiche les éléments dans l’ordre avec une flèche entre chaque cellule.
 * @param l - Pointeur vers la tête de la liste.
 */
void printListe(Liste l) {
    if (estVideListe(l)) {
        printf("Liste : [Vide]\n");
        return;
    }

    Liste courant = l;
    printf("Liste : ");

    while (courant != NULL) {
        printf("[%d]", courant->data);

        // On affiche la flèche seulement s'il y a un élément suivant
        if (courant->suivant != NULL) {
            printf(" -> ");
        }

        courant = suivantListe(courant);
    }

    printf(" -> NULL\n");
}