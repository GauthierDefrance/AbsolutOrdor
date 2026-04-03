#pragma once
#include "data/struct/processus.h"
#include "data/struct/ProcessusIterator.h"
#include "data/struct/ExecutionTimeline.h"
#include <limits.h>

/**
 * Calcul du nombre de quanta d'un certain type pour un processus
 * @param processus Le processus à analyser
 * @param operationProcessus Le type de quantum (UC, ES, W)
 * @return Nombre de quanta du type donné
 */
int nbProcesssuOperation(Processus *processus, enum OperationProcessus operationProcessus);


/**
 * Calcul du temps d'attente moyen de tous les processus
 * @param timeline La timeline complète
 * @return Temps d'attente moyen
 */
double attenteMoyProcessus(ExecutionTimeline *timeline);


/**
 * Calcul du temps de restitution (turnaround) d'un processus
 * @param processus Le processus à analyser
 * @return Temps total depuis l'arrivée jusqu'à la fin
 */
int tempsRestitutionProcessus(Processus *processus);


/**
 * Calcul du temps de restitution moyen de tous les processus
 * @param timeline La timeline complète
 * @return Turnaround moyen
 */
double restitutionMoyProcessus(ExecutionTimeline *timeline);


/**
 * Calcul du temps de réponse d'un processus
 * @param processus Le processus à analyser
 * @return Temps avant la première exécution CPU
 */
int tempsRepProcessus(Processus *processus);


/**
 * Calcul du temps de réponse moyen de tous les processus
 * @param timeline La timeline complète
 * @return Temps de réponse moyen
 */
double tempRepMoyProcessus(ExecutionTimeline *timeline);


/**
 * Calcul du taux d'occupation du CPU en pourcentage
 * @param timeline La timeline complète
 * @return Pourcentage du temps où le CPU a été utilisé
 */
double tauxOccupationCPU(ExecutionTimeline *timeline);

/**
 * @brief Calcule le temps maximal (borne supérieure) de la timeline.
 *
 * Parcourt tous les processus de la timeline, et pour chacun calcule son temps
 * de fin théorique : timeArrival + somme(nbQuantum de tous ses quanta). Le
 * résultat renvoyé est le maximum de ces temps de fin parmi tous les processus.
 *
 * @param timeline Pointeur vers la timeline d'exécution (doit contenir une liste de processus).
 * @return Le plus grand instant de fin trouvé (>= 0), ou 0 si timeline/processus est NULL.
 */
int getTimelineMax(const ExecutionTimeline *timeline);

/**
 * @brief Renvoie le premier instant (début) de la timeline.
 *
 * Recherche le plus petit timeArrival parmi tous les processus de la timeline.
 *
 * @param timeline Pointeur vers la timeline d'exécution.
 * @return Le plus petit timeArrival, ou 0 si timeline/processus est NULL ou si la liste est vide.
 */
int getTimelineDebut(const ExecutionTimeline *timeline);

/**
 * @brief Compte le nombre de processus présents dans la timeline.
 *
 * Incrémente un compteur en parcourant la liste timeline->processus.
 *
 * @param timeline Pointeur vers la timeline d'exécution.
 * @return Le nombre total de processus, ou 0 si timeline/processus est NULL.
 */
int getTimelineNbProcessus(const ExecutionTimeline *timeline);

/**
 * @brief Calcule le nombre total de ticks (unités de temps) pour un type d'opération donné.
 *
 * Parcourt tous les processus puis tous leurs quanta ; si q->type correspond au type demandé,
 * ajoute q->nbQuantum au total.
 *
 * @param timeline Pointeur vers la timeline d'exécution.
 * @param type Type d'opération à cumuler (ex: UC, ES, W).
 * @return Somme des nbQuantum pour le type indiqué, ou 0 si timeline/processus est NULL.
 */
int getTimelineTicksType(const ExecutionTimeline *timeline, enum OperationProcessus type);