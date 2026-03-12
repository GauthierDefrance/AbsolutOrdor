#pragma once
#include "liste_tq.h"
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


enum OperationProcessus {
    UC,
    ES,
    W
    // ADVICE : Ajouter vos autres algorithmes ici
};

typedef struct {
    enum OperationProcessus type;
    int nbQuantum;
} Quantum;

typedef struct {
    char name[NBMAXCHAR];	// Le nom du processus
    int timeArrival;		// Sa date d'arrivé
    ListeTQ listeTQ;
} Processus;

/*******************************************
    Allocation / Libération
********************************************/
Processus* allocMemProcessus();
void libMemProcessus(Processus *processus);

/*******************************************
    Initialisation
********************************************/
void initProcessus(Processus *processus);

/*******************************************
    Primitives
********************************************/
const char *processusName(Processus *processus);
int getTimeArrival(Processus *processus);
ListeTQ getListeTQProcessus(Processus *processus);


/*******************************************
    Console
********************************************/
void afficherProcessus(Processus *processus);