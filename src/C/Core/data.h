#pragma once
#include "structure.h"
#include "processus.h"
#include "resultat.h"



/*******************************************
    Structure de données
********************************************/
typedef struct {
    TypeDonnee type;
    union {
        Processus *processus;
        Resultat *resultat;
    } content;
} Data;



/*******************************************
    Fonction allocation mémoire
********************************************/
Data* allocMemData(TypeDonnee type);



/*******************************************
    Fonction libération mémoire
********************************************/
void libMemData(Data *data);



/******************************************
    Fonction d'initialisation
******************************************/
void initData(const Data *data);



/******************************************
    Fonction de création de donnée
******************************************/
Data* buildData(TypeDonnee type, ...);
Data* createDataProcessus(const Processus *src);
Data* createDataResultat(const Resultat *src);
Data* deepCopyData(const Data *data);


/******************************************
    Fonction primitive
*******************************************/
void traiterData(const Data *data);