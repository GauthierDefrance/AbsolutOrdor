#pragma once
#include "data/struct/ExecutionTimeline.h"
#include "data/struct/processus.h"
#include "data/struct/file.h"



// FIFO fonctionnement step by step + ajout de processus dynamiquement
// Entrée = Liste de processus à chaque STEP
// Conversion intermédiaire en FILE ?


ExecutionTimeline* fifo(ListeTQ liste_tq);