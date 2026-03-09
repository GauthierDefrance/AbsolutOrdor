#pragma once
#include "liste_tq.h"
#include "processus.h"

/**
 * Resultat sert pour afficher le resultat de nos fonctions Algo d'ordonnancement.
 * Il y a donc, un tableau de Processus de taille N, indiquant les infos de chaque processus.
 * Mais il y a aussi une ListeTQ d'éléments indiquant la position d'un processus en particulier dans le tableau.
 */


/*******************************************
    Structure de données
********************************************/
typedef struct {
    int nbProcessus;
    Processus *tabProcessus; // Tableau des processus indexé et pointé par la listeTQ
    ListeTQ *listeTQ; // Tableau d'indices de processus
} Resultat;



/*******************************************
    Fonction allocation mémoire
********************************************/
Resultat* allocMemResultat(int sizeTabProcessus);			// Retourne un pointeur, car Data est une union de pointeur



/*******************************************
    Fonction libération mémoire
********************************************/
void libMemResultat(Resultat *resultat);



/******************************************
    Fonction d'initialisation
******************************************/
void initResultat(const Resultat *resultat);



/******************************************
    Fonction primitive
*******************************************/
int nbProcess(const Resultat *resultat);
Processus *getProcess(const Resultat *resultat);
ListeTQ* getListeTQ(const Resultat *resultat);



/******************************************
    Fonction consoles
*******************************************/
void afficherResultat(const Resultat *resultat);