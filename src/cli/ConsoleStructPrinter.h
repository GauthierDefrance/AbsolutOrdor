#pragma once

#include <iostream>
#include <iomanip>
#include <string>

extern "C" {
    #include "data/struct/liste.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/processus.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/utilities/TimelineStatsCalculator.h"
}

/**
 * Affiche une ListeTQ de Processus
 * @param liste
 */
void afficherCSVCharge(ListeTQ liste);


/**
 * Affiche la Timeline
 * @param tl
 */
void afficherTimeline(const ExecutionTimeline *tl);


/**
 * Affiche la Timeline avec un décalage pour que tout les éléments soient alignés
 * @param tl
 */
void afficherTimelineAvecDecalage(const ExecutionTimeline *tl);


/**
 * Affiche le résultat des calculs des stats de la Timeline
 * @param tl
 */
void afficherStatsTimeline(const ExecutionTimeline *tl);