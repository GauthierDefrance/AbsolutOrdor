#pragma once
#include "algo_commun.h"
#include "data/struct/ExecutionTimeline.h"
/**
 * @file sjrf.h
 * @brief Interface de l'ordonnanceur Shortest Job Remaining First (SJRF).
 */


/**
 * @brief Exécute l'ordonnancement Shortest Job Remaining First (SJRF).
 *
 * Cet algorithme est la variante préemptive du SJF. Il garantit que le processus
 * ayant le temps d'exécution restant le plus court occupe toujours le CPU.
 * Un changement de contexte se produit dès qu'un processus plus court arrive
 * ou sort d'une phase d'E/S.
 *
 * @param liste  La liste des processus à ordonnancer (ListeTQ).
 * @param taille Le nombre total de processus dans le système.
 * @return ExecutionTimeline* Un pointeur vers l'historique de l'exécution (à libérer après l'usage).
 */
ExecutionTimeline* sjrf(ListeTQ liste, int taille);


/**
 * @brief Gère l'avancement des processus en Entrées/Sorties pour le SJRF.
 *
 * Met à jour les compteurs de temps en E/S et réinsère les processus
 * terminés dans la file d'attente pour qu'ils soient réévalués par le CPU.
 *
 * @param fileES             Pointeur vers la file des processus bloqués en E/S.
 * @param fileAttente        File des processus prêts (UC).
 * @param resultat           Pointeur vers la structure de résultats.
 * @param nbProcessusTraiter Compteur de progression de la simulation.
 * @param temps_courant      Horloge actuelle du système.
 */
void traiterES_SJRF(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter, int temps_courant);