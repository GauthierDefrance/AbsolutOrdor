#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * La structure processus sert à la représentation d'un processus.
 * Elle est utilisé pour traduire le CSV en code C.
 */



#define NBMAXCHAR 50


/*******************************************
    Structure de données
********************************************/
typedef struct {
    char name[NBMAXCHAR];	// Le nom du processus
    int timeArrival;		// Sa date d'arrivé
    int nbQuantum;       	// Son nombre de quantums / ticks nécéssaire avant la fin du programme
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



/******************************************
    Fonction consoles
*******************************************/

void afficherProcessus(const Processus *processus);