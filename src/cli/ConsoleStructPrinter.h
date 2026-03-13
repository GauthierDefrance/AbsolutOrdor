#pragma once

#include <iostream>
#include <iomanip>
#include <string>

extern "C" {
#include "data/struct/liste.h"
#include "data/struct/liste_tq.h"
#include "data/struct/processus.h"
#include "data/struct/ExecutionTimeline.h"
}


void afficherCSVCharge(ListeTQ liste);
void afficherTimeline(ExecutionTimeline *tl);
void afficherTimelineAvecDecalage(ExecutionTimeline *tl);