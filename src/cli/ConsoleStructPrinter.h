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


void afficherCSVCharge(ListeTQ liste);
void afficherTimeline(const ExecutionTimeline *tl);
void afficherTimelineAvecDecalage(const ExecutionTimeline *tl);
void afficherStatsTimeline(const ExecutionTimeline *tl);