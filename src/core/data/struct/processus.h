#pragma once
#include "liste_tq.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * @file processus.h
 * @brief Définition et manipulation de la structure de données Processus.
 */

/**
 * Constante indiquant la longueuer maximale des noms des processus
 */
#define NBMAXCHAR 50


/*******************************************
    Structure de données
********************************************/


/**
 * @enum OperationProcessus
 * @brief Types d'états possibles pour un quantum de temps.
 */
enum OperationProcessus {
    UC,
    ES,
    W
};


/**
 * @struct Quantum
 * @brief Plus petite unité de temps atomique d'un état donné.
 */
typedef struct {
    enum OperationProcessus type;
    int nbQuantum;
} Quantum;


/**
 * @struct Processus
 * @brief Représentation statique d'un processus système.
 *
 * Contient les métadonnées (nom, arrivée) et la séquence complète de son
 * cycle de vie sous forme d'une liste chaînée de quanta.
 */
typedef struct {
    char name[NBMAXCHAR];
    int timeArrival;
    ListeTQ listeTQ;
} Processus;


/*******************************************
    Allocation / Libération
********************************************/


/**
 * @brief Alloue la mémoire brute pour une structure Processus.
 *
 * @return Processus* Un pointeur vers la structure, ou NULL si échec malloc.
 */
Processus* allocMemProcessus();

/**
 * @brief Libère récursivement la mémoire d'un processus.
 * Détruit également la liste de quanta associée pour éviter les fuites.
 *
 * @param processus Le processus à supprimer.
 */
void libMemProcessus(Processus *processus);

/**
 * @brief Alloue et initialise un nouveau Quantum.
 */
Quantum *allocQuantum(enum OperationProcessus type, int nbQuantum);


/*******************************************
    Initialisation
********************************************/


/**
 * @brief Initialise les champs d'un processus déjà alloué.
 *
 * Crée une liste de quanta vide par défaut.
 */
void initProcessus(Processus *processus);


/*******************************************
    Primitives
********************************************/


/** @return Le nom du processus (lecture seule). */
const char *processusName(const Processus *processus);
/** @return La date d'arrivée du processus. */
int getTimeArrival(const Processus *processus);
/** @return La liste des opérations (Quanta) du processus. */
ListeTQ getListeTQProcessus(Processus *processus);

/**
 * @brief Ajoute une opération à la fin du cycle de vie.
 *
 * Si la dernière opération est de même type, les durées sont fusionnées.
 * Sinon, un nouveau quantum est créé et ajouté en queue.
 *
 * @param liste La liste à modifier.
 * @param type Type d'opération (UC/ES/W).
 * @param n Nombre de ticks à ajouter.
 */
void pushOrMergeOperationProcessus(ListeTQ liste, enum OperationProcessus type, int n);

/**
 * @brief Crée une copie intégrale et indépendante d'un processus.
 * Utile pour lancer plusieurs simulations sur les mêmes données sans interférences.
 */
Processus *deepCopyProcessus(Processus *src);

/**
 * @brief Copie uniquement l'enveloppe du processus (Nom et Arrivée).
 * La liste des quanta de la copie sera vide.
 */
Processus *deepCopyProcessusWithoutQuantums(Processus *src);


/*******************************************
    Console
********************************************/


/** @brief Affiche les détails du processus et son cycle de vie dans la console. */
void afficherProcessus(Processus *processus);