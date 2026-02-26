/**
* @file resultat.c
 * @brief Implémentation des fonctions de gestion des résultats.
 */


#include "resultat.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour une structure Resultat.
 * @note  Alloue la structure Resultat ainsi que le tableau de Processus
 *        associé à la taille fournie.
 * @param sizeTabProcessus - Nombre de processus à allouer dans le tableau.
 * @return Resultat* - Pointeur vers le Resultat alloué, NULL en cas d’échec.
 */
Resultat* allocMemResultat(int sizeTabProcessus) {
    Resultat *res = (Resultat *) malloc(sizeof(Resultat));
    if ( res == NULL ) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
        return NULL;
    }

    Processus *process = (Processus *) calloc(sizeTabProcessus,sizeof(Processus));

    if ( process == NULL ) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
        free(res);
        return NULL;
    }

    res->nbProcessus = sizeTabProcessus;
    res->tabProcessus = process;

    return res;
}



/*******************************************
    Fonction libération mémoire
********************************************/
/**
 * @brief Libère la mémoire associée à un Resultat.
 * @note  Ne libère pas les structures internes pointées par Resultat
 *        (ex. tabProcessus, listeTQ).
 * @param resultat - Pointeur vers le Resultat à libérer.
 */
void libMemResultat(Resultat *resultat) {
    if (resultat==NULL) return;
    free(resultat);
    resultat = NULL;
}



/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise les champs d’un Resultat à leurs valeurs par défaut.
 * @note  Cette fonction ne modifie pas les champs déjà initialisés
 *        lors de l’allocation mémoire.
 * @param resultat - Pointeur vers le Resultat à initialiser.
 */
void initResultat(const Resultat *resultat) {
    if (resultat == NULL) return;

}

/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Retourne le nombre de processus stockés dans un Resultat.
 * @param resultat - Pointeur vers le Resultat.
 * @return int - Nombre de processus.
 */
int nbProcess(const Resultat *resultat) { return resultat->nbProcessus;}

/**
 * @brief Retourne le tableau de Processus associé à un Resultat.
 * @note  Le tableau est alloué dynamiquement lors de allocMemResultat().
 * @param resultat - Pointeur vers le Resultat.
 * @return Processus* - Pointeur vers le tableau de Processus.
 */
Processus *getProcess(const Resultat *resultat) { return resultat->tabProcessus; }

/**
 * @brief Retourne la liste Tête-Queue associée à un Resultat.
 * @param resultat - Pointeur vers le Resultat.
 * @return ListeTQ* - Pointeur vers la ListeTQ associée.
 */
ListeTQ* getListeTQ(const Resultat *resultat) { return resultat->listeTQ; }