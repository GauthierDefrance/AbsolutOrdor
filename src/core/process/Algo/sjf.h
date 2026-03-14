#pragma once
#include "data/struct/ProcessusIterator.h"
#include "data/struct/file.h"
#include "data/struct/ExecutionTimeline.h"
// ltq
// liste
// processus




ExecutionTimeline* sjf(ListeTQ liste, int taille);
ListeTQ trieListe(ListeTQ ltq);
ProcessusIterator* estMinTempsUCProcessus(File* f);
void traiterUC(ProcessusIterator** surLeCPU,File *fileAttente, File *fileES, ExecutionTimeline *resultat, int *nbProcessusTraiter);
void traiterES(File *fileES, File *fileAttente, ExecutionTimeline *resultat, int *nbProcessusTraiter);
void traiterWait(File *fileAttente, ExecutionTimeline *resultat);
void afficherResultatsSJF(ExecutionTimeline *tl);