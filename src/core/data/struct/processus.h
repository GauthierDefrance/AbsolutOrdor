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
Quantum *allocQuantum(enum OperationProcessus type, int nbQuantum);

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
void pushOrMergeOperationProcessus(ListeTQ liste, enum OperationProcessus type, int n);

/**
 * Crée une copie profonde d'un Processus.
 * Chaque Quantum est alloué indépendamment.
 * À libérer avec libMemProcessus().
 */
Processus *deepCopyProcessus(Processus *src);
Processus *deepCopyProcessusWithoutQuantums(Processus *src);


/*******************************************
    Console
********************************************/
void afficherProcessus(Processus *processus);