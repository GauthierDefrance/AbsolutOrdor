#pragma once
#include "data.h"



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
    //struct Cellule *precedent; ADVICE : Ajout possible si on a besoin de souvent supprimer en queue
    Data data;					//Pointeur vers une donnée générique, precisé par TypeDonee
} *ListeGenerique;


/*******************************************
    Fonction allocation mémoire
********************************************/
ListeGenerique allocListeGenerique();



/*******************************************
    Fonction libération mémoire
********************************************/
void libereCelluleCourante(ListeGenerique cellule);


/******************************************
    Fonction de suppression
*******************************************/
void destroyListeGenerique(ListeGenerique liste);
void suppTete(ListeGenerique *liste);
void suppQueue(ListeGenerique *liste);



/******************************************
    Fonction d'initialisation
******************************************/
void initListeGenerique(ListeGenerique cellule);
ListeGenerique createListe(const Data *data);
void setCelluleData(ListeGenerique cellule, const Data *newData);


/******************************************
    Fonction primitive
*******************************************/
ListeGenerique suivantListe(ListeGenerique liste);
ListeGenerique queueListe(ListeGenerique liste);		// WARNING : Complexité O(N) éviter son utilisation
bool estVideListe(ListeGenerique l);



/******************************************
    Fonction d'insertion
*******************************************/
void inserTete(ListeGenerique *l, const Data *data);
void inserQueue(ListeGenerique *l, const Data *data);



/******************************************
    Fonction de parcours
*******************************************/
void parcoursListeGenerique(ListeGenerique l);



/******************************************
    Fonction d'affichage console
*******************************************/
void printListeGenerique(ListeGenerique l);