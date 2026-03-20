#pragma once
#include "sjrf.h"
/**
 * @file lottery_scheduling.h
 * @brief Déclarations pour l'algorithme d'ordonnancement par loterie.
 */


/**
 * @brief Exécute la simulation complète de l'ordonnancement Lottery Scheduling.
 *
 * @param liste La liste des processus chargés (ListeTQ).
 * @param taille Le nombre total de processus.
 * @return ExecutionTimeline* Le résultat de la simulation pour affichage du Gantt.
 */
ExecutionTimeline* lottery_scheduling(ListeTQ liste, int taille);

/**
 * @brief Sélectionne aléatoirement le prochain processus à exécuter.
 *
 * @param f Pointeur vers la file d'attente des processus prêts (Ready Queue).
 * @return ProcessusIterator* Le processus élu, ou NULL si la file est vide.
 */
ProcessusIterator* tirageGagnant(const File *f);

/**
 * @brief Gère l'incrémentation du temps d'attente pour les processus en file.
 *
 * @param fileAttente La file contenant les processus en attente de CPU.
 * @param resultat La timeline où enregistrer les états 'W'.
 * @param temps_courant Le tick actuel de la simulation.
 */
void traiterWait_lottery(const File *fileAttente, const ExecutionTimeline *resultat, int temps_courant);