#pragma once
#include "sjrf.h"


ExecutionTimeline* lottery_scheduling(ListeTQ liste, int taille);
ProcessusIterator* tirageGagnant(const File *f);
void traiterWait_lottery(const File *fileAttente, const ExecutionTimeline *resultat, int temps_courant);