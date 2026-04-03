/**
 * @file liste.c
 * @brief Implémentation des fonctions de gestion de la liste générique chaînée.
 *
 * Ce module fournit les briques de base pour manipuler des listes simples.
 * L'utilisation de pointeurs (void *) pour les données permet une structure
 * polymorphe capable de stocker n'importe quel type d'objet simulé.
 */

#include "liste.h"


/*******************************************
    Fonction allocation mémoire
********************************************/


/**
 * @brief Alloue la mémoire brute pour une nouvelle cellule de liste.
 *
 * Cette fonction effectue un appel à malloc et initialise les pointeurs internes
 * à NULL pour éviter les accès mémoire invalides.
 *
 * @return Liste Le pointeur vers la cellule allouée, ou NULL si le système
 * n'a plus de mémoire disponible.
 */
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


/**
 * @brief Supprime la cellule en tête de la liste.
 *
 * Cette fonction décroche le premier maillon, met à jour la tête de la liste
 * chez l'appelant via le double pointeur, et libère la mémoire.
 *
 * @param l Adresse du pointeur de tête (Liste *). Permet de modifier la variable originale.
 * @param freeData  Pointeur de fonction pour libérer la donnée (void *).
 *
 * Passer NULL si la donnée ne doit pas être libérée.
 */
void suppTete(Liste *l, void (*freeData)(void *)) {
    if (l == NULL || *l == NULL) return;

    Liste tmp = *l;
    *l = tmp->suivant;

    if (freeData && tmp->data)
        freeData(tmp->data);

    free(tmp);
}


/**
 * @brief Supprime la dernière cellule de la liste.
 *
 * @details Complexité temporelle : **O(N)** car il faut parcourir la liste
 * pour trouver l'avant-dernier maillon afin de mettre son champ 'suivant' à NULL.
 *
 * @param l Adresse du pointeur de tête.
 * @param freeData  Callback pour la libération de la donnée stockée.
 */
void suppQueue(Liste *l, void (*freeData)(void *)) {
    if (l == NULL || *l == NULL) return;

    // Cas particulier : un seul élément dans la liste
    if ((*l)->suivant == NULL) {
        if (freeData && (*l)->data)
            freeData((*l)->data);
        free(*l);
        *l = NULL;
        return;
    }

    // Parcours pour trouver l'avant-dernier maillon
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

/**
 * @brief Libère récursivement l'ensemble de la liste.
 * @param liste La tête de la liste à détruire.
 * @param freeData Callback de libération des données. Indispensable si
 * les données ont été allouées dynamiquement.
 */
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


/**
 * @brief Initialise une cellule déjà allouée.
 *
 * @param cellule La cellule dont on souhaite réinitialiser le chaînage.
 */
void initListe(Liste cellule) {
    if (cellule == NULL) return;
    cellule->suivant = NULL;
}


/******************************************
    Fonction de création
******************************************/


/**
 * @brief Alloue une cellule et initialise son contenu.
 *
 * @param data Pointeur vers la donnée à encapsuler dans le maillon.
 * @return Liste La nouvelle cellule initialisée, ou NULL en cas d'erreur.
 */
Liste createListe(void *data) {
    Liste cellule = allocListe();
    if (!cellule) return NULL;
    cellule->data = data;
    return cellule;
}


/**
 * @brief Met à jour la donnée d'un maillon existant.
 *
 * @param cellule Le maillon cible.
 * @param data    Le nouveau pointeur de donnée à stocker.
 */
void setCelluleData(Liste cellule, void *data) {
    if (cellule == NULL) return;
    cellule->data = data;
}


/******************************************
    Fonction primitive
******************************************/


/**
 * @brief Accède au maillon suivant.
 *
 * @param cellule Le maillon actuel.
 * @return Liste Le maillon suivant, ou NULL si fin de liste.
 */
Liste suivantListe(Liste cellule) {
    if (cellule == NULL) return NULL;
    return cellule->suivant;
}


/**
 * @brief Localise le dernier maillon de la liste.
 *
 * @param l La tête de la liste.
 * @return Liste Le dernier maillon, ou NULL si la liste est vide.
 */
Liste queueListe(Liste l) {
    if (l == NULL) return NULL;
    while (l->suivant != NULL) {
        l = l->suivant;
    }
    return l;
}


/**
 * @brief Détermine si la liste contient des éléments.
 *
 * @param l La tête de liste.
 * @return true si la liste est vide (NULL), false sinon.
 */
bool estVideListe(Liste l) {
    return l == NULL;
}


/**
 * @brief Récupère la donnée contenue dans un maillon.
 *
 * @param l Le maillon cible.
 * @return void* Le pointeur générique stocké.
 */
void *donneeListe(Liste l) {
    if (l == NULL) return NULL;
    return l->data;
}


/**
 * @brief Compte le nombre de maillons présents dans la liste.
 *
 * @param l La tête de liste.
 * @return int Le nombre d'éléments (0 si la liste est vide).
 */
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


/**
 * @brief Insère une nouvelle donnée au tout début de la liste.
 *
 * @param l Adresse du pointeur de tête (permet de changer la tête).
 * @param data La donnée à insérer.
 */
void inserTete(Liste *l, void *data) {
    if (l == NULL) return;

    Liste cellule = createListe(data);
    if (cellule == NULL) return;

    cellule->suivant = *l;
    *l = cellule;
}


/**
 * @brief Ajoute une donnée à la fin de la liste.
 *
 * @param l Adresse du pointeur de tête.
 * @param data La donnée à insérer.
 */
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


/**
 * @brief Affiche graphiquement la structure de la liste dans la console.
 *
 * @details Si printData n'est pas fourni, la fonction affiche l'adresse
 * mémoire brute des pointeurs pour permettre un débogage rapide.
 *
 * @param l La tête de la liste.
 * @param printData Fonction de rappel permettant d'afficher proprement
 *
 * le type de donnée stocké (ex: nom du processus).
 */
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