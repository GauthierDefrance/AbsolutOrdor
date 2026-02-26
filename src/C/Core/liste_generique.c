/**
 * @file liste_generique.c
 * @brief Implémentation des fonctions de gestion de la liste générique chaînée.
 */

#include "liste_generique.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour une cellule de ListeGenerique.
 * @return ListeGenerique - Pointeur vers la cellule allouée, NULL en cas d'échec.
 */
ListeGenerique allocListeGenerique() {
    ListeGenerique cellule = (ListeGenerique) malloc(sizeof(struct Cellule));
    if (cellule == NULL) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire d'une cellule\n");
        return NULL;
    }
    return cellule;
}


/*******************************************
    Fonction libération mémoire
********************************************/
/**
 * @brief Libère la mémoire de la cellule courante et de sa donnée interne.
 * @note  Libère d'abord la donnée via libMemData, puis la cellule elle-même.
 * @param cellule - Pointeur vers la cellule à libérer.
 */
void libereCelluleCourante(ListeGenerique cellule) {
    if (cellule == NULL) return;
    libMemData(&(cellule->data));
    free(cellule);
}


/******************************************
    Fonction de destruction
******************************************/
/**
 * @brief Détruit complètement une ListeGenerique en libérant toutes ses cellules.
 * @note  Utilise suppTete pour libérer chaque cellule en cascade.
 *        Pas de free final, car suppTete libère déjà chaque cellule.
 * @param liste - Pointeur vers la tête de la liste à détruire.
 */
void destroyListeGenerique(ListeGenerique liste) {
    if (liste == NULL) return;
    while (liste != NULL) {
        suppTete(&liste);
    }
}


/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise le pointeur suivant d'une cellule à NULL.
 * @param cellule - Pointeur vers la cellule à initialiser.
 */
void initListeGenerique(ListeGenerique cellule) {
    if (cellule == NULL) return;
    cellule->suivant = NULL;
}


/******************************************
    Fonction de création
******************************************/
/**
 * @brief Crée une nouvelle cellule et y copie la donnée fournie.
 * @param data - Pointeur vers la donnée à copier dans la cellule.
 * @return ListeGenerique - Pointeur vers la cellule créée, NULL en cas d'échec.
 */
ListeGenerique createListe(const Data *data) {
    ListeGenerique cellule = allocListeGenerique();
    if (!cellule) return NULL;

    initListeGenerique(cellule);

    if (data != NULL) {
        Data *copie = deepCopyData(data);
        if ( !copie ) {
            free(cellule);
            return NULL;
        }
        cellule->data = *copie;   // copie structurelle
        free(copie);              // on peut libérer la structure temporaire
    }

    return cellule;
}


/**
 * @brief Écrase et libère l'ancienne donnée d'une cellule, puis y copie la nouvelle.
 * @param cellule - Pointeur vers la cellule à modifier.
 * @param newData - Pointeur vers la nouvelle donnée à copier.
 */
void setCelluleData(ListeGenerique cellule, const Data *newData) {
    if (cellule == NULL || newData == NULL) return;
    libMemData(&(cellule->data));

    Data* copie = deepCopyData(newData);
    if ( copie == NULL ) return;
    cellule->data = *copie;
    free(copie);
}


/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Retourne la cellule suivante dans la liste.
 * @param cellule - Pointeur vers la cellule courante.
 * @return ListeGenerique - Pointeur vers la cellule suivante, NULL si fin de liste.
 */
ListeGenerique suivantListe(ListeGenerique cellule) {
    if (cellule == NULL) return NULL;
    return cellule->suivant;
}

/**
 * @brief Retourne la dernière cellule de la liste.
 * @warning Complexité O(N) — préférer queueLTQ() qui est O(1).
 * @param l - Pointeur vers la tête de la liste.
 * @return ListeGenerique - Pointeur vers la dernière cellule.
 */
ListeGenerique queueListe(ListeGenerique l) {
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
bool estVideListe(ListeGenerique l) { return l == NULL; }


/******************************************
    Fonction d'insertion
*******************************************/
/**
 * @brief Insère une donnée en tête de la liste.
 * @param l    - Adresse du pointeur vers la tête de la liste.
 * @param data - Pointeur vers la donnée à insérer.
 */
void inserTete(ListeGenerique *l, const Data *data) {
    if (data == NULL || l == NULL) return;
    ListeGenerique liste = createListe(data);
    if (liste == NULL) return;

    liste->suivant = *l;
    *l = liste;
}

/**
 * @brief Insère une donnée en queue de la liste.
 * @warning Complexité O(N) — préférer inserQueueLTQ() qui est O(1).
 * @param l    - Adresse du pointeur vers la tête de la liste.
 * @param data - Pointeur vers la donnée à insérer.
 */
void inserQueue(ListeGenerique *l, const Data *data) {
    if (data == NULL || l == NULL) return;
    ListeGenerique liste = createListe(data);
    if (liste == NULL) return;

    if (*l == NULL) {
        *l = liste;
        return;
    }

    ListeGenerique queue = queueListe(*l);
    queue->suivant = liste;
}


/******************************************
    Fonction de suppression
*******************************************/
/**
 * @brief Supprime la cellule en tête de la liste et libère sa mémoire.
 * @param l - Adresse du pointeur vers la tête de la liste.
 */
void suppTete(ListeGenerique *l) {
    if (estVideListe(*l)) return;
    ListeGenerique tmp = *l;
    *l = suivantListe(*l);
    libereCelluleCourante(tmp);
}

/**
 * @brief Supprime la cellule en queue de la liste et libère sa mémoire.
 * @note  Parcourt la liste jusqu'à l'avant-dernier élément.
 *        Complexité O(N).
 * @param l - Adresse du pointeur vers la tête de la liste.
 */
void suppQueue(ListeGenerique *l) {
    if (l == NULL || *l == NULL) return;

    if (suivantListe(*l) == NULL) {
        libereCelluleCourante(*l);
        *l = NULL;
        return;
    }

    ListeGenerique courant = *l;
    while (suivantListe(suivantListe(courant)) != NULL) {
        courant = suivantListe(courant);
    }

    libereCelluleCourante(suivantListe(courant));
    courant->suivant = NULL;
}


/******************************************
    Fonction de parcours
*******************************************/
/**
 * @brief Parcourt la liste et affiche chaque donnée via traiterData.
 * @param l - Pointeur vers la tête de la liste.
 */
void parcoursListeGenerique(ListeGenerique l) {
    while (l != NULL) {
        traiterData(&l->data);
        l = suivantListe(l);
    }
}