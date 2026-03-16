#pragma once
#include "algo_commun.h"


#include "data/struct/ExecutionTimeline.h"


ExecutionTimeline* sjf(ListeTQ liste, int taille);
ProcessusIterator* retirerMinTempsUC(const File* f);