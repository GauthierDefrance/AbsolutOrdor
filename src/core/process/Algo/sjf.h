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
void afficherResultatsSJF(ExecutionTimeline *tl);