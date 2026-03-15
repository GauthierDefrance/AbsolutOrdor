#pragma once
#include "data/struct/ProcessusIterator.h"
#include "data/struct/file.h"
#include "data/struct/ExecutionTimeline.h"


ExecutionTimeline* sjf(ListeTQ liste, int taille);
ListeTQ trieListe(ListeTQ ltq);
ProcessusIterator* retirerMinTempsUC(const File* f);