#include "structure_processeur.h"


/*******************************************
	Fonction allocation mémoire - Générique
********************************************/
TGenerique allocMem_processus() {
	TGenerique lp;
	lp = (TGenerique) calloc(1, sizeof(CelluleProcessus));
	return lp;
}


TGenerique allocMem_resultat(){
	TGenerique lr;
	lr = (TGenerique) calloc(1, sizeof(CelluleResultat));
	return lr;
}


TGenerique allocMem(FonctionAlloc alloc){
	TGenerique cellule = alloc();
	if ( cellule == NULL ) {
		fprintf(stderr, "Erreur : echec de l'allocation memoire pour les processus\n");
	} else {
		printf("allocation memoire reussi\n");
	}
	return cellule;
}



/*******************************************
	Fonction libération mémoire - Générique
********************************************/
void libListeMem(TGenerique *l, FonctionSuivant suivant){
	// condition d'arrêt
	printf("libMem en cours");
	if ( *l == NULL ) {
		printf("\t - element deja NULL - libMem terminer\n");	
		return;
	}

	// liberation mémoire pour chaque élement de la liste
	TGenerique tmp;
	while ( ! est_videListe(*l) ) {
		tmp = *l;
		*l = suivant(*l);	// On avance
		free(tmp);			// libération de l'élement sauvegarder
	}

	*l = NULL;
	printf("\t - libMem terminer\n");
}



/******************************************
	Fonction d'initialistion - Générique
******************************************/
TGenerique initListe(){ return NULL; }



/******************************************
	Fonction primitive - Générique
*******************************************/
bool est_videListe(TGenerique l){ return l == NULL; }

TGenerique suivant_processus(TGenerique cellule){
	if ( cellule == NULL ) return NULL;  
	return (TGenerique)((CelluleProcessus *)cellule)->suivant;
}


TGenerique suivant_resultat(TGenerique cellule) {
	if ( cellule == NULL ) return NULL;  
	return (TGenerique)((CelluleResultat *)cellule)->suivant;
}


TGenerique queueListe(TGenerique l, FonctionSuivant suivant) {
	/* Renvoie le dernier element de la liste chainée */
	while ( !est_videListe(suivant(l)) ) {
		l = suivant(l);
	}
	return l;
}


void traiterProcessus(TGenerique *l) {
	if ( est_videListe(*l) ) {
		printf("La ListeProcessus est vide\n");
		return;
	}
	printf("le moteur du processus : %s tourne vroum vroum\n", processus_name((ListeProcessus)*l) );
}


void traiterResultat(TGenerique *l) {
	if ( est_videListe(*l) ) { 
		printf("La ListeResultat est vide\n");
		return;
	}
	printf("resultat : %d\n", ((CelluleResultat *)*l)->donnee );
}



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
	CelluleProcessus *cellule = (CelluleProcessus *) alloc();

	// remplissage des données
	strcpy(cellule->processus_name, name);	// copie une chaine de char
	cellule->time_arrival = timeArrival;
	cellule->nb_quantum = nbQuantum;
	cellule->suivant = (CelluleProcessus *) *l;
	*l = (TGenerique *) cellule;
}

void inserResultatTete(TGenerique *l, FonctionAlloc alloc, int donnee){
	CelluleResultat *cellule = (CelluleResultat *) alloc();

	// remplissage des données
	cellule->donnee = donnee;
	cellule->suivant = (CelluleResultat *) *l;
	*l = (TGenerique *) cellule; 
}

void inserProcessusQueue(TGenerique *l, FonctionAlloc alloc, const char *name, int timeArrival, int nbQuantum){
	if ( est_videListe(*l) ) {
		inserProcessusTete(l, alloc, name, timeArrival, nbQuantum);
	} else {
		CelluleProcessus *der = (CelluleProcessus *) queueListe(*l, suivant_processus); 
		inserProcessusTete((TGenerique *)&der->suivant, alloc, name, timeArrival, nbQuantum);
	}
}

void inserResultatQueue(TGenerique *l, FonctionAlloc alloc, int donnee){
	if ( est_videListe(*l) ) {
		inserResultatTete(l , alloc, donnee);
	} else {
		CelluleResultat *der = (CelluleResultat *) queueListe(*l, suivant_resultat); 
		inserResultatTete((TGenerique *)&der->suivant, alloc, donnee);
	}
}



/******************************************
	Fonction de suppression
*******************************************/
void suppTete(TGenerique *l, FonctionSuivant suivant){
	if ( est_videListe(*l) ) { return; }
	TGenerique tmp;
	tmp = *l;
	*l = suivant(*l);
	free(tmp);
}

void suppProcessusQueue(TGenerique *l, FonctionSuivant suivant){
	// si la liste est vide on supprime pas
	if ( est_videListe(*l) ){ return; }

	// si la liste contient 1 element
	if ( est_videListe(suivant(*l)) ) {
		suppTete(l, suivant);
		return;
	}

	/*
	* On avance jusqu'à l'avant dernier
	* pour pouvoir faire le chainage vers NULL
	*/
	TGenerique tmp = *l;
	while ( !est_videListe(suivant(suivant(tmp))) ) {
		tmp = suivant(tmp);
	}

	// on libère le dernier
	free(suivant(tmp));
	((CelluleProcessus *)tmp)->suivant = NULL;	
}

void suppResultatQueue(TGenerique *l, FonctionSuivant suivant){
	// si la liste est vide on supprime pas
	if ( est_videListe(*l) ){ return; }

	// si la liste contient 1 element
	if ( est_videListe(suivant(*l)) ) {
		suppTete(l, suivant);
		return;
	}

	/*
	* On avance jusqu'à l'avant dernier
	* pour pouvoir faire le chainage vers NULL
	*/
	TGenerique tmp = *l;
	while ( !est_videListe(suivant(suivant(tmp))) ) {
		tmp = suivant(tmp);
	}

	// on libère le dernier
	free(suivant(tmp));
	((CelluleResultat *)tmp)->suivant = NULL;
}