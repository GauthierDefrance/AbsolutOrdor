#pragma once
#include "processus.h"
#include "liste_tq.h"
#include "data/struct/ProcessusIterator.h"

// Forward declaration pour éviter l'include circulaire
typedef struct ProcessusIterator ProcessusIterator;

/**
 * ExecutionTimeline :
 * Représente l'état final de l'ordonnancement.
 * C'est une ListeTQ de Processus, où chaque Processus
 * possède une listeTQ de Quantum (UC, ES, W) complète.
 */
typedef struct {
    ListeTQ processus; // ListeTQ de Processus*
} ExecutionTimeline;


/*******************************************
    Allocation / Libération
********************************************/
ExecutionTimeline *allocTimeline();
void destroyTimeline(ExecutionTimeline *tl);


/*******************************************
    Construction
********************************************/

/**
 * Initialise la timeline avec une copie (sans quantums) de chaque processus.
 * L'ordre d'insertion est l'ordre d'arrivée dans la ListeTQ.
 */
void initTimelineProcessus(ExecutionTimeline *timeline, ListeTQ liste_tq);

/**
 * Ajoute un processus (déjà ordonnancé) dans la timeline.
 */
void ajouterProcessusTimeline(ExecutionTimeline *tl, Processus *p);


/*******************************************
    Recherche
********************************************/

/**
 * Retourne le Processus* dans la timeline correspondant à l'iterator donné.
 * Recherche par comparaison de nom.
 */
Processus *getTimelineProcessus(ExecutionTimeline *timeline, ProcessusIterator *it);