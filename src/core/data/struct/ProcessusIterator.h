#pragma once
#include "data/struct/processus.h"
#include <stdbool.h>

/**
 * ProcessusIterator :
 * Structure temporaire utilisée pour simuler l’avancement
 * d’un processus dans sa liste de quantum.
 *
 * Il garde :
 * - une référence vers le processus associé
 * - un pointeur vers le quantum courant
 * - le temps restant dans ce quantum
 */
typedef struct {
    Processus *processus;     // Le processus auquel il est associé
    Liste quantumCourant;     // pointeur vers la cellule Quantum* actuelle
    int tempsRestant;         // temps restant dans ce quantum
} ProcessusIterator;

/**
 * Initialise un iterator pour un processus donné.
 * Doit être appelé avant la simulation.
 */
void initIterator(Processus *p, ProcessusIterator *it);

/**
 * Retourne l’état courant du processus (UC, ES, W).
 * Si quantumCourant == NULL → le processus est terminé.
 */
enum OperationProcessus etatIterator(ProcessusIterator *it);

/**
 * Avance l’iterator d’un tick :
 * - décrémente le quantum courant
 * - passe au quantum suivant si nécessaire
 */
void avancerIterator(ProcessusIterator *it);

/**
 * Indique si le processus a terminé tous ses quantum.
 */
bool iteratorEstFini(ProcessusIterator *it);
