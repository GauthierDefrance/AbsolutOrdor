#pragma once
#include "data/struct/processus.h"
#include "data/struct/resultat.h"




Resultat* sjf(Processus tab[], int taille);
int nbQuantumTotal(Processus tab[], int taille);
int estMinQuantum(Processus tab[], bool processusTraiter[], int taille, int temps_courant);
int MinTimeArrival(Processus tab[], bool processusTraiter[], int taille);