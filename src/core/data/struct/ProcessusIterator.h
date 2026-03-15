#pragma once
#include "data/struct/processus.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * ProcessusIterator :
 * Structure temporaire utilisée pour simuler l'avancement
 * d'un processus dans sa liste de quantum.
 *
 * Il garde :
 * - une référence vers le processus associé
 * - un pointeur vers le quantum courant
 * - le temps restant dans ce quantum
 */
typedef struct ProcessusIterator {
    Processus *processus;   // Le processus auquel il est associé
    Liste quantumCourant;   // Pointeur vers la cellule Quantum* actuelle
    int tempsRestant;       // Temps restant dans ce quantum
    bool enAttente; //True = le processus attend
} ProcessusIterator;


/*******************************************
    Initialisation
********************************************/

/**
 * Initialise un iterator pour un processus donné.
 * Doit être appelé avant la simulation.
 */
void initIterator(Processus *p, ProcessusIterator *it);

/**
 * Crée un tableau de ProcessusIterator depuis une ListeTQ de Processus*.
 * Le tableau est à libérer avec free() par l'appelant.
 *
 * @param listeTQ  ListeTQ de Processus*
 * @param outSize  [OUT] nombre d'éléments dans le tableau retourné
 * @return         tableau alloué, ou NULL si erreur / liste vide
 */
ProcessusIterator *createTabIteratorFromLTQ(ListeTQ listeTQ, int *outSize);


/*******************************************
    Primitives
********************************************/

/**
 * Retourne l'état courant du processus (UC, ES, W).
 * Si quantumCourant == NULL → le processus est terminé, retourne -1.
 */
enum OperationProcessus etatIterator(ProcessusIterator *it);

/**
 * Avance l'iterator d'un tick :
 * - décrémente tempsRestant
 * - passe au quantum suivant si tempsRestant atteint 0
 */
Quantum *avancerIterator(ProcessusIterator *it);

/**
 * Indique si le processus a terminé tous ses quantums.
 */
bool iteratorEstFini(ProcessusIterator *it);


bool enAttenteIterator(const ProcessusIterator *it);


/*******************************************
    Utilitaires
********************************************/

/**
 * Compte les iterators non finis dont le processus est déjà arrivé à `time`.
 *
 * @param tab   tableau d'iterators
 * @param n     taille du tableau
 * @return      nombre d'iterators actifs
 */
int countAlive(ProcessusIterator *tab, int n);