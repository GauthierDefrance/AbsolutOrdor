#include "structure_processeur.h"


/*******************************************
	Fonction allocation mémoire - Générique
********************************************/
TGenerique allocMem_processus() {
TGenerique lp;
	lp = (TGenerique) malloc(sizeof(CelluleProcessus));
	return lp;
}


TGenerique allocMem_resultat(){
TGenerique lr;
	lr = (TGenerique) malloc(sizeof(CelluleResultat));
	return lr;
}


TGenerique allocMem(FonctionAlloc alloc){
	TGenerique cellule = alloc();
	if ( cellule == NULL ) {
		fprintf(stderr, "Erreur : échec de l'allocation mémoire pour les processus\n");
	}
	return cellule;
}



/*******************************************
	Fonction libération mémoire - Générique
********************************************/
void libListeMem(TGenerique *l, FonctionSuivant suivant){
	// condition d'arrêt
	if ( *l == NULL ) { return; }

	// liberation mémoire pour chaque élement de la liste
	TGenerique tmp;
	while ( ! est_videListe(*l) ) {
		tmp = *l;
		*l = suivant(*l);	// On avance
		free(tmp);			// libération de l'élement sauvegarder
	}

	*l = NULL;
}



/******************************************
	Fonction d'initialistion - Générique
******************************************/
TGenerique initListe(){
	return NULL;
}



/******************************************
	Fonction primitive - Générique
*******************************************/
TGenerique suivant_processus(TGenerique cellule){ return (TGenerique)((CelluleProcessus *)cellule)->suivant; }
TGenerique suivant_resultat(TGenerique cellule) { return (TGenerique)((CelluleResultat *)cellule)->suivant; }


TGenerique dernierListe(TGenerique l, FonctionSuivant suivant) {
	/*
	* Renvoie le dernier element de la liste chainée
	*/
	while ( !est_videListe(suivant(l)) ) {
		l = suivant(l);
	}
	return l;
}


bool est_videListe(TGenerique l){ return l == NULL; }


void traiterProcessus(TGenerique *l) { printf("le moteur du processus tourne vroum vroum\n"); }
void traiterResultat(TGenerique *l) { printf(" resultat : %d\n", ((CelluleResultat *)*l)->donnee ); }



/******************************************
	Fonction primitive - ListeProcessus
*******************************************/
char *processus_name(ListeProcessus l){	return l->processus_name; }
int time_arrival(ListeProcessus l){	return l->time_arrival; }
int nb_quantum(ListeProcessus l){ return l->nb_quantum; }



/******************************************
	Fonction primitive - ListeResultat
*******************************************/
int donnee(ListeResultat l){ return l->donnee; }



/******************************************
	Fonction d'insertion
*******************************************/
void inserProcessusTete(TGenerique *l, FonctionAlloc alloc, const char *name, int timeArrival, int nbQuantum){
	CelluleProcessus *cp = (CelluleProcessus *) alloc();

	// remplissage des données
	strcpy(cp->processus_name, name);	// copie une chaine de char
	cp->time_arrival = timeArrival;
	cp->nb_quantum = nbQuantum;
	cp->suivant = (CelluleProcessus *)*l;
	*l = (TGenerique)cp;
}

void inserResultatTete(TGenerique *l, FonctionAlloc alloc, int donnee){
	CelluleResultat *cr = (CelluleResultat *) alloc();

	// remplissage des données
	cr->donnee = donnee;
	cr->suivant = (CelluleResultat *)*l;
	*l = (TGenerique)cr;
}

/*
void inserProcessusQueue(ListeProcessus *l, int elt);
void inserResultatQueue(ListeResultat *l, int elt);

*/

/******************************************
	Fonction de suppression
*******************************************//*
void suppProcessusTete(ListeProcessus *l);
void suppProcessusQueue(ListeProcessus *l);

void suppResultatTete(ListeResultat *l);
void suppResultatQueue(ListeResultat *l);*/

/*

À l'appel :
ListeP = (ListeProcessus)initListe();
ListeP = (ListeResultat)initListe();
libListeMem((TGenerique *)&ListeP, suivant_processus);
libListMem((TGenerique *)&ListeR, suivant_resultat);

*/