#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*******************************************
    Structure de données
********************************************/
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
    int data;
} *Liste;


/*******************************************
    Fonction allocation mémoire
********************************************/
Liste allocListe();

/******************************************
    Fonction de suppression
*******************************************/
void destroyListe(Liste liste);
void suppTete(Liste *liste);
void suppQueue(Liste *liste);



/******************************************
    Fonction d'initialisation
******************************************/
void initListe(Liste cellule);
Liste createListe(int elem);
void setCelluleData(Liste cellule, int elem);


/******************************************
    Fonction primitive
*******************************************/
Liste suivantListe(Liste liste);
Liste queueListe(Liste liste);		// WARNING : Complexité O(N) éviter son utilisation
bool estVideListe(Liste l);
int donneeListe(Liste l);


/******************************************
    Fonction d'insertion
*******************************************/
void inserTete(Liste *l, int elem);
void inserQueue(Liste *l, int elem);


/******************************************
    Fonction d'affichage console
*******************************************/
void printListeGenerique(Liste l);