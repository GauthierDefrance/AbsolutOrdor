#pragma once
#include "data/struct/ExecutionTimeline.h"
#include "data/struct/processus.h"
#include "algo_commun.h"
#include "data/struct/file.h"
/**
 * @file fifo.h
 * @brief Interface de l'ordonnanceur First-In, First-Out (FIFO).
 */


/**
 * @brief Exécute la simulation d'ordonnancement selon l'algorithme FIFO.
 *
 * Les processus sont traités dans leur ordre d'arrivée. Un processus conserve
 * le CPU jusqu'à ce qu'il termine sa rafale de calcul ou qu'il doive effectuer une E/S.
 *
 * @param liste_tq Liste des processus à ordonnancer.
 * @return ExecutionTimeline* Chronologie complète de l'exécution (à libérer).
 */
ExecutionTimeline* rrn(ListeTQ liste_tq,int nb);


/**
 * @brief Alloue le CPU au processus situé en tête de file pour un tick.
 *
 * @param file La file d'attente des processus prêts (Ready Queue).
 * @param timeline Structure de résultats pour enregistrer l'activité CPU.
 */
void executerTeteFile_RRN(File file, const ExecutionTimeline *timeline,int *nb);