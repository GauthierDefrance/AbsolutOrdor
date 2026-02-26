#pragma once
#include "structure.h"



/*******************************************
    Structure de données
********************************************/
typedef struct {
    int donnee;
} Resultat;



/*******************************************
    Fonction allocation mémoire
********************************************/
Resultat* allocMemResultat();			// Retourne un pointeur, car Data est une union de pointeur



/*******************************************
    Fonction libération mémoire
********************************************/
void libMemResultat(Resultat **resultat);				// Passe directement la structure non pointée en paramètre



/******************************************
    Fonction d'initialisation
******************************************/
void initResultat(Resultat *resultat);



/******************************************
    Fonction primitive
*******************************************/
int donnee(Resultat resultat);
void traiterResultat(const Resultat *resultat);