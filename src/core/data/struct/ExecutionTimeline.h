#pragma once
#include "processus.h"
#include "liste_tq.h"

/**
 * ExecutionTimeline :
 * Représente l’état final de l’ordonnancement.
 * C’est une ListeTQ de Processus, où chaque Processus
 * possède une listeTQ de Quantum (UC, ES, W) complète.
 */
typedef struct {
    ListeTQ processus; // ListeTQ de Processus*
} ExecutionTimeline;

ExecutionTimeline *allocTimeline();
void destroyTimeline(ExecutionTimeline *tl);

/**
 * Construit une timeline à partir de la liste de processus d’entrée
 * (issue du CSV) et d’un ordonnanceur.
 * L’ordonnanceur va insérer les W, réordonner les UC/ES, etc.
 */
ExecutionTimeline *genererTimeline(ListeTQ processusInitial);

void ajouterProcessusTimeline(ExecutionTimeline *tl, Processus *p);



/**
 * Affiche, pour chaque processus, sa séquence de quantum finale.
 */
void afficherTimeline(ExecutionTimeline *tl);
