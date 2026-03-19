#pragma once
#include "data/struct/processus.h"
#include "data/struct/ProcessusIterator.h"
#include "data/struct/ExecutionTimeline.h"

/**
 * Calcul du nombre de quanta d'un certain type pour un processus
 * @param processus      Le processus à analyser
 * @param operationProcessus   Le type de quantum (UC, ES, W)
 * @return               Nombre de quanta du type donné
 */
int nbProcesssuOperation(Processus *processus, enum OperationProcessus operationProcessus);


/**
 * Calcul du temps d'attente moyen de tous les processus
 * @param timeline       La timeline complète
 * @return               Temps d'attente moyen
 */
double attenteMoyProcessus(ExecutionTimeline *timeline);


/**
 * Calcul du temps de restitution (turnaround) d'un processus
 * @param processus      Le processus à analyser
 * @return               Temps total depuis l'arrivée jusqu'à la fin
 */
int tempsRestitutionProcessus(Processus *processus);


/**
 * Calcul du temps de restitution moyen de tous les processus
 * @param timeline       La timeline complète
 * @return               Turnaround moyen
 */
double restitutionMoyProcessus(ExecutionTimeline *timeline);


/**
 * Calcul du temps de réponse d'un processus
 * @param processus      Le processus à analyser
 * @return               Temps avant la première exécution CPU
 */
int tempsRepProcessus(Processus *processus);


/**
 * Calcul du temps de réponse moyen de tous les processus
 * @param timeline       La timeline complète
 * @return               Temps de réponse moyen
 */
double tempRepMoyProcessus(ExecutionTimeline *timeline);


/**
 * Calcul du taux d'occupation du CPU en pourcentage
 * @param timeline       La timeline complète
 * @return               Pourcentage du temps où le CPU a été utilisé
 */
double tauxOccupationCPU(ExecutionTimeline *timeline);