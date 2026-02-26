/**
* @file resultat.c
 * @brief Implémentation des fonctions de gestion des résultats.
 */


#include "resultat.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour un Resultat.
 * @note  Retourne un pointeur, car Data est une union de pointeurs.
 * @return Resultat* - Pointeur vers le Resultat alloué, NULL en cas d'échec.
 */
Resultat* allocMemResultat() {
    Resultat *res = (Resultat *) malloc(sizeof(Resultat));
    if ( res == NULL ) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
        return NULL;
    }
    return res;
}



/*******************************************
    Fonction libération mémoire
********************************************/
/**
 * @brief Libère la mémoire d'un Resultat et met le pointeur à NULL.
 * @note  Prend un double pointeur pour que le NULL remonte à l'appelant.
 * @param resultat - Adresse du pointeur vers le Resultat à libérer.
 */
void libMemResultat(Resultat **resultat) {
    if (*resultat==NULL) return;
    free(*resultat);
    *resultat = NULL;
}



/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise les champs d'un Resultat à leurs valeurs par défaut.
 * @param resultat - Pointeur vers le Resultat à initialiser.
 */
void initResultat(Resultat *resultat) {
    if (resultat == NULL) return;
    resultat->donnee = 0;
}

/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Retourne la donnée du résultat.
 * @param resultat - Le Resultat (par valeur).
 * @return int - La donnée contenue dans le Resultat.
 */
int donnee(Resultat resultat){ return resultat.donnee; }

/**
 * @brief Affiche les informations d'un Resultat dans la sortie standard.
 * @param resultat - Pointeur vers le Resultat à afficher.
 */
void traiterResultat(const Resultat *resultat) {
    if ( resultat == NULL ) {
        printf("Le Resultat est vide\n");
        return;
    }
    printf("resultat : %d\n",
        donnee(*resultat)
    );
}