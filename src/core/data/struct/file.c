/**
 * @file file.c
 * @brief Implémentation d'une file (Queue) générique de type FIFO.
 *
 * Ce module encapsule une ListeTQ pour garantir des opérations d'enfilage
 * et de défilage performantes. Il est utilisé pour la gestion des files
 * d'attente des processus dans l'ordonnanceur.
 */


#include "file.h"
#include <stdlib.h>
#include <stdio.h>


/*******************************************
    Allocation / Initialisation
********************************************/


/**
 * @brief Alloue et initialise la structure de contrôle d'une File.
 *
 * @details Cette fonction alloue la structure `File` ainsi que son conteneur
 * interne `ListeTQ`.
 *
 * @return File Le pointeur vers la file initialisée, ou NULL en cas d'échec
 * d'allocation mémoire.
 */
File allocFile() {
    File f = malloc(sizeof(struct File_Cellule));
    if (!f) {
        fprintf(stderr, "Erreur : allocation File\n");
        return NULL;
    }

    f->ltq = allocMemLTQ();
    if (!f->ltq) {
        free(f);
        return NULL;
    }

    initLTQ(f->ltq);
    return f;
}


/**
 * @brief Réinitialise une file existante à un état vide.
 *
 * @param f La file à réinitialiser.
 */
void initFile(File f) {
    if (!f) return;
    initLTQ(f->ltq);
}


/*******************************************
    Opérations principales
********************************************/


/**
 * @brief Ajoute un élément à la fin de la file (Enqueue).
 *
 * @details L'insertion se fait en queue de la structure ListeTQ interne
 * pour respecter la logique Premier Entré, Premier Sorti.
 *
 * @param f    La file cible.
 * @param data Pointeur générique vers la donnée à stocker.
 */
void enfilerFile(File f, void *data) {
    if (!f) return;
    inserQueueLTQ(f->ltq, data);   // FIFO : on ajoute en queue
}

/**
 * @brief Retire et retourne l'élément en tête de la file (Dequeue).
 *
 * @warning Cette fonction retire le maillon de la file mais ne libère pas
 * la donnée pointée par 'data'. La libération de la donnée est de la
 * responsabilité de l'appelant.
 *
 * @param f La file cible.
 * @return void* Le pointeur vers la donnée qui était en tête, ou NULL si la file est vide.
 */
void *defilerFile(File f) {
    if (!f || estVideFile(f)) return NULL;

    Liste tete = f->ltq->tete;
    void *data = tete->data;

    // suppression sans libérer data
    suppTeteLTQ(f->ltq, NULL);

    return data;
}


/**
 * @brief Consulte la donnée en tête de file sans la retirer.
 *
 * @param f La file à consulter.
 * @return void* La donnée en tête, ou NULL si la file est vide.
 */
void *teteFile(File f) {
    if (!f || estVideFile(f)) return NULL;
    return f->ltq->tete->data;
}


/**
 * @brief Vérifie si la file ne contient aucun élément.
 *
 * @param f La file à tester.
 * @return true Si la file est vide ou inexistante, false sinon.
 */
bool estVideFile(File f) {
    return (!f || f->ltq->tete == NULL);
}


/**
 * @brief Calcule le nombre d'éléments actuellement dans la file.
 *
 * @details Cette fonction délègue le comptage à la couche inférieure (Liste).
 *
 * @param f La file cible.
 * @return int Le nombre d'éléments (0 si la file est vide).
 */
int tailleFile(File f) {
    // Vérification de sécurité : si la file ou la liste interne n'existe pas
    if (!f || !f->ltq) {
        return 0;
    }

    // On délègue le calcul à la fonction tailleListe existante
    // en passant la tête de liste contenue dans le ListeTQ
    return tailleListe(f->ltq->tete);
}

/*******************************************
    Destruction
********************************************/


/**
 * @brief Détruit intégralement la file et libère la mémoire associée.
 *
 * @details Libère la structure ListeTQ interne, les maillons, et optionnellement
 * les données via le callback freeData.
 *
 * @param f La file à détruire.
 * @param freeData Callback de libération des données utilisateur.
 */
void destroyFile(File f, void (*freeData)(void *)) {
    if (!f) return;

    destroyLTQ(f->ltq, freeData);
    free(f);
}
