#pragma once
#include "liste.h"


/**
 * Liste TQ Générique regroupant,
 * les listes de Resultat et les listes de Processus
 */
typedef struct {
    Liste tete;
    Liste queue;
} *ListeTQ;


/*******************************************
    Fonction allocation mémoire
********************************************/
ListeTQ allocMemLTQ();



/*******************************************
    Fonction libération mémoire
********************************************/
void libMemLTQ(ListeTQ *ltq);


/******************************************
    Fonction de suppression
*******************************************/
void destroyLTQ(ListeTQ listeTQ);
void suppTeteLTQ(ListeTQ listeTQ);
void suppQueueLTQ(ListeTQ listeTQ);



/******************************************
    Fonction d'initialisation
******************************************/
void initLTQ(ListeTQ listeTQ);



/******************************************
    Fonction primitive
*******************************************/
Liste teteLTQ(ListeTQ listeTQ);
Liste queueLTQ(ListeTQ listeTQ);			// ADVICE : Choisissez celui-là si vous devez aller à la fin de la liste



/******************************************
    Fonction d'insertion
*******************************************/
void inserTeteLTQ(ListeTQ listeTQ, int elem);
void inserQueueLTQ(ListeTQ listeTQ, int elem);