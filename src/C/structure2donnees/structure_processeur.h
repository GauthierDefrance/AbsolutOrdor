#ifndef STRUCTURE_PROCESSEUR
#define STRUCTURE_PROCESSEUR

#define NBMAXCHAR 50

#include <stdlib.h>   // malloc, free
#include <stdio.h>    // printf, fprintf
#include <string.h>   // strcpy
#include <stdbool.h>  // bool


/***************************************
	Strucutre de données
***************************************/

/*
Structure d'une Liste chainée
avec dans chaque cellule les informations d'un processus
*/
typedef struct CelluleProcessus{
	char processus_name[NBMAXCHAR];		// chaine non modifiable
	int time_arrival;
	int nb_quantum;
	struct CelluleProcessus *suivant;
}*ListeProcessus;

typedef struct CelluleProcessus CelluleProcessus;

/*
Structure qui permet de stocker les informations
sortante des algorithmes
*/
typedef struct CelluleResultat{
	int donnee;
	struct CelluleResultat *suivant;
}*ListeResultat;

typedef struct CelluleResultat CelluleResultat;


typedef struct {
	ListeProcessus tete;
	ListeProcessus queue;
} *ListePTQ;


typedef struct {
	ListeResultat tete;
	ListeResultat queue;
} *ListeRTQ;


/*
* Type générique - c'est un pointeur universel (void *)
* Permet à une fonction de manipuler n'importe quel type de données
* sans en connaître la structure. Le cast vers un type réel
* est à la charge des fonctions concrètes appelantes.
*/
typedef void *TGenerique;


/*
* Déclaration d'un type "FonctionSuivant"
* Ce type représente un pointeur vers une fonction
* qui prend en parametre une cellule quelconque (TGenerique cellule)
* et retourn la cellule suivante (TGenerique)
*/
typedef TGenerique (*FonctionSuivant)(TGenerique cellule);
typedef TGenerique (*FonctionAlloc)();



/*******************************************
	Fonction allocation mémoire - Générique
********************************************/
TGenerique allocMem_processus();
TGenerique allocMem_resultat();
TGenerique allocMem(FonctionAlloc alloc);



/*******************************************
	Fonction libération mémoire - Générique
********************************************/
void libListeMem(TGenerique *l, FonctionSuivant suivant);



/******************************************
	Fonction d'initialistion - Générique
******************************************/
TGenerique initListe();



/******************************************
	Fonction de création de cellule
******************************************/
ListeProcessus creatNouvelleCelluleProcessus(const char *name, int timeArrival, int nbQuantum);
ListeResultat creatNouvelleCelluleResultat(int donnee);



/******************************************
	Fonction primitive - Générique
*******************************************/
TGenerique suivant_processus(TGenerique cellule);
TGenerique suivant_resultat(TGenerique cellule);
TGenerique queueListe(TGenerique l, FonctionSuivant suivant);


bool est_videListe(TGenerique l);


void traiterProcessus(TGenerique *l);
void traiterResultat(TGenerique *l);



/******************************************
	Fonction primitive - ListeProcessus
*******************************************/
char *processus_name(ListeProcessus l);
int time_arrival(ListeProcessus l);
int nb_quantum(ListeProcessus l);



/******************************************
	Fonction primitive - ListeResultat
*******************************************/
int donnee(ListeResultat l);



/******************************************
	Fonction d'insertion
*******************************************/
void inserProcessusTete(TGenerique *l, FonctionAlloc alloc, const char *name, int timeArrival, int nbQuantum);
void inserResultatTete(TGenerique *l, FonctionAlloc alloc, int donnee);


void inserProcessusQueue(TGenerique *l, FonctionAlloc alloc, const char *name, int timeArrival, int nbQuantum);
void inserResultatQueue(TGenerique *l, FonctionAlloc alloc, int donnee);



/******************************************
	Fonction de suppression
*******************************************/
void suppTete(TGenerique *l, FonctionSuivant suivant);
void suppProcessusQueue(TGenerique *l, FonctionSuivant suivant);
void suppResultatQueue(TGenerique *l, FonctionSuivant suivant);

#endif