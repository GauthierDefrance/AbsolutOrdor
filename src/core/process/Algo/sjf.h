#pragma once
#include "algo_commun.h"
#include "data/struct/ExecutionTimeline.h"
/**
 * @file sjf.h
 * @brief Interface de l'ordonnanceur Shortest Job First (SJF) non-préemptif.
 */


/**
 * @brief Exécute la simulation d'ordonnancement selon l'algorithme SJF.
 *
 * Le SJF sélectionne le processus ayant le cycle de calcul (UC) le plus court.
 * Contrairement au SJRF, cet algorithme est non-préemptif : une fois qu'un processus
 * a le CPU, il ne peut pas être interrompu avant la fin de sa rafale de calcul.
 *
 * @param liste La liste des processus à ordonnancer (ListeTQ).
 * @param taille Le nombre total de processus dans le système.
 * @return ExecutionTimeline* Historique de l'exécution (mémoire à libérer par l'appelant).
 */
ExecutionTimeline* sjf(ListeTQ liste, int taille);


/**
 * @brief Gère la progression des processus en phase d'E/S pour le SJF.
 *
 * @param[in,out] fileES File des processus bloqués en E/S.
 * @param[in,out] fileAttente File des processus prêts à revenir sur le CPU.
 * @param[in]     resultat Chronologie pour l'enregistrement des états.
 * @param[in,out] nbProcessusTraiter Compteur de processus ayant terminé leur cycle de vie.
 */
void traiterES_SJF(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter);