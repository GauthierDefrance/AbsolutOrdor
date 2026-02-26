#pragma once
#include "structure.h"

#define NBMAXCHAR 50


/*******************************************
    Structure de données
********************************************/
typedef struct {
    char name[NBMAXCHAR];	// Le nom du processus
    int time_arrival;		// Sa date d'arrivé
    int nb_quantum;       	// Son nombre de quantums / ticks nécéssaire avant la fin du programme
} Processus;



/*******************************************
    Fonction allocation mémoire
********************************************/
Processus* allocMemProcessus();



/*******************************************
    Fonction libération mémoire
********************************************/
void libMemProcessus(Processus **processus);			// Passe directement la structure non pointée en paramètre



/******************************************
    Fonction d'initialisation
******************************************/
void initProcessus(Processus *processus);



/******************************************
    Fonction primitive
*******************************************/
const char *processusName(const Processus *processus);
int timeArrival(Processus processus);
int nbQuantum(Processus processus);
void traiterProcessus(const Processus *processus);