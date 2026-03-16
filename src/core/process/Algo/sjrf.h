#pragma once
#include "algo_commun.h"
#include "data/struct/ExecutionTimeline.h"


ExecutionTimeline* sjrf(ListeTQ liste, int taille);
ProcessusIterator* retirerMinTempsUCSJRF(const File* f);