#pragma once

#include "data/struct/processus.h"
#include <stdbool.h>

typedef struct TabProcessus_s {
    int nbProcess;
    Processus *tabProcess;
} *TabProcessus;

// Allocation mémoire
TabProcessus allocTabProcessus(int size);

// Libération mémoire
void freeTabProcessus(TabProcessus *tabProcessus);

// Lecture du CSV et génération du tableau
TabProcessus createTabProcessusFromCSV(char *fileName);

// Fonctions utilitaires
bool isCommentLine(FILE *f);
void skipLine(FILE *f);
void skipAllIgnoredLines(FILE *f);
bool isTabProcessusValid(TabProcessus tab);