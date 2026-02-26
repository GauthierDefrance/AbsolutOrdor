/**
* @file processus.c
 * @brief Implémentation des fonctions de gestion des processus.
 */


#include "processus.h"


/*******************************************
    Fonction allocation mémoire
********************************************/
/**
 * @brief Alloue la mémoire pour un Processus.
 * @note  Retourne un pointeur, car Data est une union de pointeurs.
 *        Name[NBMAXCHAR] est inclus dans sizeof (Processus),
 *        son initialisation se fait via initProcessus().
 * @return Processus* - Pointeur vers le Processus alloué, NULL en cas d'échec.
 */
Processus* allocMemProcessus() {
    Processus *p = (Processus *) malloc( sizeof(Processus));
    if ( p == NULL ) {
        fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
        return NULL;
    }
    // name[NBMAXCHAR] inclus dans sizeof(Processus), initialisation via initProcessus()
    return p;
}



/*******************************************
    Fonction libération mémoire
********************************************/
/**
 * @brief Libère la mémoire d'un Processus et met le pointeur à NULL.
 * @note  Prend un double pointeur pour que le NULL remonte à l'appelant.
 * @param processus - Adresse du pointeur vers le Processus à libérer.
 */
void libMemProcessus(Processus **processus) {
    if (*processus == NULL) return;
    free(*processus);
    *processus = NULL;
}



/******************************************
    Fonction d'initialisation
******************************************/
/**
 * @brief Initialise les champs d'un Processus à leurs valeurs par défaut.
 * @note  strncpy est préféré à strcpy car il évite les dépassements de buffer.
 * @param processus - Pointeur vers le Processus à initialiser.
 */
void initProcessus(Processus *processus) {
    if (processus == NULL) return;
    strncpy(processus->name, "", NBMAXCHAR-1); // Advice : il vaut mieux préférer strncpy à strcpy, strcpy s'arrête uniquement quand il voit '\0' dans la chaîne de chars
    processus->time_arrival = 0;
    processus->nb_quantum = 0;
}



/******************************************
    Fonction primitive
*******************************************/
/**
 * @brief Retourne le nom du processus.
 * @param l - Le Processus (par valeur).
 * @return char* - Pointeur vers le tableau name.
 */
const char *processusName(const Processus *l) {
    if (l==NULL) return NULL;
    return l->name;
}

/**
 * @brief Retourne la date d'arrivée du processus.
 * @param l - Le Processus (par valeur).
 * @return int - La date d'arrivée.
 */
int timeArrival(Processus l){	return l.time_arrival; }

/**
 * @brief Retourne le nombre de quantums du processus.
 * @param l - Le Processus (par valeur).
 * @return int - Le nombre de quantums.
 */
int nbQuantum(Processus l){ return l.nb_quantum; }


/**
 * @brief Affiche les informations d'un Processus dans la sortie standard.
 * @param l - Pointeur vers le Processus à afficher.
 */
void traiterProcessus(const Processus *l) {
    if ( l == NULL ) {
        printf("Le Processus est vide\n");
        return;
    }
    printf("[PROCESSUS] Nom : %s | Arrivee : %d | Quantum : %d\n",
        processusName(l),
        timeArrival(*l),
        nbQuantum(*l)
    );
}