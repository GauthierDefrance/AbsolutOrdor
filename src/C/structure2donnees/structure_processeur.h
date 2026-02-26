#ifndef STRUCTURE_PROCESSEUR
#define STRUCTURE_PROCESSEUR

#define NBMAXCHAR 50

#include <stdlib.h>   // malloc, free
#include <stdio.h>    // printf, fprintf
#include <string.h>   // strcpy
#include <stdbool.h>  // bool
#include <stdarg.h>   // fonctions à plusieurs arguments

/***************************************
	Strucutre de données
***************************************/


/**
 * Type générique - c'est un pointeur universel (void *)
 * Permet à une fonction de manipuler n'importe quel type de données
 * sans en connaître la structure. Le cast vers un type réel
 * est à la charge des fonctions concrètes appelantes.
 * */
typedef void *TGenerique;



/***************************************
	DATA TYPES
***************************************/
typedef enum {
	TYPE_PROCESSUS,
	TYPE_RESULTAT
} TypeDonnee;



/***************************************
	STRUCTURES OF THE DATA TYPES
***************************************/

typedef struct {
	char name[NBMAXCHAR];	// Le nom du processus
	int time_arrival;		// Sa date d'arrivé
	int nb_quantum;       	// Son nombre de quantums / ticks nécéssaire avant la fin du programme
} Processus;


typedef struct {
	int donnee;				// La donnée contenue par Résultat
} Resultat;


typedef union {
	Processus *processus;
	Resultat *resultat;
	// ADVICE : On peut ajouter d'autres structures de données au besoin
} Data;


/***************************************
	STRUCTURES OF THE GENERAL LIST
***************************************/


/**
 * Cette structure de ListeGenerique permet d'avoir
 * une méthode généralisée pour le fonctionnement
 * des listes de processus ou resultat ou
 * encore autres types éventuels.
 *
 * Attention, si vous ne faites pas attention,
 * vous pouvez mélanger liste de résultats et de processus.
 */
typedef struct Cellule {
	struct Cellule *suivant;
	TypeDonnee type;			//Enum indiquant la donnée comprise dans notre type
	Data data;					//Pointeur vers une donnée générique, precisé par TypeDonee
} *ListeGenerique;



/**
 * Liste TQ Générique regroupant,
 * les listes de Resultat et les listes de Processus
 */
typedef struct {
	ListeGenerique tete;
	ListeGenerique queue;
} *ListeTQ;




/*******************************************
	Fonction allocation mémoire
********************************************/
ListeTQ allocMemLTQ();
Data allocMemData(TypeDonnee type);
Processus allocMemProcessus();
Resultat allocMemResultat();



/*******************************************
	Fonction libération mémoire
********************************************/
void libMemLTQ(ListeTQ ltq);
void libMemeData(Data data);
void libMemeProcessus(Processus processus);
void libMemeResultat(Resultat resultat);



/******************************************
	Fonction d'initialistion
******************************************/
void initLTQ(ListeTQ listeTQ);
void initData(Data *data);
void initProcessus(Processus *processus);
void initResultat(Resultat *resultat);



/******************************************
	Fonction de destruction
******************************************/
void destroyLTQ(ListeTQ listeTQ);
void destroyListeGenerique(ListeGenerique liste);



/******************************************
	Fonction de création de donnée
******************************************/
Processus createProcessus(const char *name, int timeArrival, int nbQuantum);
Resultat createResultat(int donnee);
Data createData(TypeDonnee type, int n, ...);		// prend n arguments (...) et un Type de Donnée



/******************************************
	Fonction primitive
*******************************************/
ListeGenerique suivantListe(ListeGenerique liste);
ListeGenerique queueListe(ListeGenerique liste);		// WARNING : Complexité O(N) éviter son utilisation
ListeGenerique teteListeTQ(ListeTQ listeTQ);
ListeGenerique queueListeTQ(ListeTQ listeTQ);			// ADVICE : Choissisez celui-là si vous devez aller à la fin de la liste


bool est_videListe(ListeGenerique l);

void traiterData(Data *data);
void traiterProcessus(Processus *processus);
void traiterResultat(Resultat *resultat);




/******************************************
	Fonction primitive - Processus
*******************************************/
char *processusName(Processus processus);
int timeArrival(Processus processus);
int nbQuantum(Processus processus);



/******************************************
	Fonction primitive - Resultat
*******************************************/
int donnee(Resultat resultat);



/******************************************
	Fonction d'insertion
*******************************************/
void inserTete(ListeGenerique l, Data *data);
void inserQueue(ListeGenerique l, Data *data);
void inserTeteLTQ(ListeTQ listeTQ, Data *data);
void inserQueueLTQ(ListeTQ listeTQ, Data *data);



/******************************************
	Fonction de suppression
*******************************************/
void suppTete(ListeGenerique liste);
void suppTeteLTQ(ListeTQ listeTQ);
void suppQueue(ListeGenerique liste);
void suppQueueLTQ(ListeTQ listeTQ);



/******************************************
	Fonction de parcours
*******************************************/
void parcoursListe(ListeGenerique l);

#endif