#pragma once
#include "data/struct/ExecutionTimeline.h"
#include "data/struct/processus.h"
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
ExecutionTimeline* fifo(ListeTQ liste_tq);


/**
 * @brief Alloue le CPU au processus situé en tête de file pour un tick.
 *
 * @param file La file d'attente des processus prêts (Ready Queue).
 * @param timeline Structure de résultats pour enregistrer l'activité CPU.
 */
void executerTeteFile(File file, const ExecutionTimeline *timeline);


/**
 * @brief Gère l'insertion et l'attente des processus en phase de calcul (UC).
 * @param it Iterateur du processus à traiter.
 * @param pTimeline Pointeur vers le processus dans la chronologie.
 * @param file File d'attente où insérer le processus.
 */
void traiterUC_FIFO(ProcessusIterator *it, const Processus *pTimeline, File file);


/**
 * @brief Gère la progression des processus en phase d'Entrées/Sorties.
 * @param it Iterateur du processus effectuant l'E/S.
 * @param pTimeline Pointeur vers le processus dans la chronologie.
 */
void traiterES_FIFO(ProcessusIterator *it, const Processus *pTimeline);