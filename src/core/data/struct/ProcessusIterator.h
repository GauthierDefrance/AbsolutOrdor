#pragma once
#include "data/struct/processus.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * @file ProcessusIterator.h
 * @brief Gestionnaire d'état dynamique pour la simulation de processus.
 */


/**
 * @struct ProcessusIterator
 * @brief Structure agissant comme un curseur sur le cycle de vie d'un processus.
 *
 * Contrairement à la structure Processus qui est statique, l'Iterator stocke
 * l'état réel à un instant T : quel quantum est en cours, combien de temps
 * il reste avant la prochaine transition et si le processus est bloqué.
 */
typedef struct ProcessusIterator {
    Processus *processus;   // Le processus auquel il est associé
    Liste quantumCourant;   // Pointeur vers la cellule Quantum* actuelle
    int tempsRestant;       // Temps restant dans ce quantum
    bool enAttente; //True = le processus attend
    int tempsEntreeFile;
} ProcessusIterator;


/*******************************************
    Initialisation
********************************************/


/**
 * @brief Initialise un itérateur pour un processus spécifique.
 *
 * Positionne le curseur sur le tout premier quantum (généralement une phase UC).
 *
 * @param p Pointeur vers le processus à itérer.
 * @param it Pointeur vers l'itérateur à initialiser.
 */
void initIterator(Processus *p, ProcessusIterator *it);

/**
 * @brief Convertit une liste de processus en un tableau d'itérateurs prêts pour la simulation.
 *
 * @param listeTQ La liste source contenant les Processus*.
 * @param outSize [OUT] Pointeur vers l'entier qui recevra le nombre d'éléments créés.
 * @return ProcessusIterator* Un tableau alloué dynamiquement (à libérer avec free()).
 */
ProcessusIterator *createTabIteratorFromLTQ(ListeTQ listeTQ, int *outSize);


/*******************************************
    Primitives
********************************************/


/**
 * @brief Identifie l'opération actuellement réalisée par le processus.
 *
 * Retourne l'état courant du processus (UC, ES, W).
 * Si quantumCourant == NULL → le processus est terminé, retourne -1.
 *
 * @param it L'itérateur concerné.
 * @return enum OperationProcessus (UC, ES, W) ou -1 si le processus est terminé.
 */
enum OperationProcessus etatIterator(ProcessusIterator *it);


/**
 * @brief Simule l'écoulement d'une unité de temps (tick).
 *
 * Décrémente le temps restant. Si le quantum est épuisé, bascule automatiquement
 * sur le quantum suivant de la liste.
 *
 * @param it L'itérateur à faire progresser.
 * @return Quantum* Le quantum qui vient de se terminer, ou NULL si le quantum actuel continue.
 */
Quantum *avancerIterator(ProcessusIterator *it);

/**
 * @brief Vérifie si le processus a épuisé l'intégralité de son cycle de vie.
 *
 * Indique si le processus a terminé tous ses quantums.
 *
 * @param it L'itérateur à tester.
 * @return true si tous les quanta ont été traités, false sinon.
 */
bool iteratorEstFini(ProcessusIterator *it);

/**
 * @brief Prédicat indiquant si le processus est marqué en attente.
 */
bool enAttenteIterator(const ProcessusIterator *it);


/*******************************************
    Utilitaires
********************************************/


/**
 * @brief Compte le nombre de processus encore actifs dans la simulation.
 *
 * Compte les iterators non finis dont le processus est déjà arrivé à `time`.
 *
 * @param tab Tableau des itérateurs de la simulation.
 * @param n   Nombre total d'itérateurs dans le tableau.
 * @return int Nombre de processus n'ayant pas encore atteint l'état "Fini".
 */
int countAlive(ProcessusIterator *tab, int n);