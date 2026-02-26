#include "structure_processeur.h"


/*******************************************
	Fonction allocation mémoire - Générique
********************************************/
/**
 * Effectue l'allocation mémoire de la liste des processus
 * avec un calloc et du type CelluleProcessus.
 * 
 * @returns {TGenerique} lp : la liste chainée allouée ou NULL en cas d'échec
 * */
TGenerique allocMem_processus() {
	TGenerique lp;
	lp = (TGenerique) calloc(1, sizeof(CelluleProcessus));
	if ( lp == NULL ) {
		fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
	} 
	return lp;
}


/**
 * Effectue l'allocation mémoire de la liste chainée de résultat
 * avec un calloc et du type CelluleResultat.
 * 
 * @returns {TGenerique} lr : la liste chainée allouée ou NULL en cas d'échec
 * */
TGenerique allocMem_resultat(){
	TGenerique lr;
	lr = (TGenerique) calloc(1, sizeof(CelluleResultat));
		if ( lr == NULL ) {
		fprintf(stderr, "Erreur : echec de l'allocation memoire\n");
	} 
	return lr;
}



/*******************************************
	Fonction libération mémoire - Générique
********************************************/
/**
 * La fonction permet de faire la libération mémoire d'une liste chainée.
 * Elle vérifie si la liste n'est pas déjà nulle pour effectuer la libération de mémoire sur toute la liste.
 * Pour cela, on parcourt toute la liste tant qu'elle n'est pas vide et on libère chaque cellule.
 * 
 * @param {TGenerique *} l : la liste chainée à liberer
 * @param {FonctionSuivant} suivant : pointeur de fonction retournant un TGenerique associé au suivant de la liste chainée
 * 										(ex: suivant_processus, suivant_resultat)
 * */
void libListeMem(TGenerique *l, FonctionSuivant suivant){
	// condition d'arrêt
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
}



/******************************************
	Fonction d'initialistion - Générique
******************************************/
/**
 * Permet de faire l'initialisation d'une liste chainée quelconque en la mettant à NULL.
 * 
 * @returns {TGenerique} : la liste chainée initialisée à NULL
 * */
TGenerique initListe(){ return NULL; }



/******************************************
	Fonction primitive - Générique
*******************************************/
/**
 * Vérifie si une liste chainée est vide (NULL)
 * 
 * @param {TGenerique} l : la liste chainée à vérifiér
 * @returns {bool} : true (1) si vide ou false (0) sinon
 * */
bool est_videListe(TGenerique l){ return l == NULL; }


/**
 * Récupère le pointeur vers la cellule suivante
 * d'une cellule appartenant à la liste des processus,
 * à condition que la cellule n'est pas vide (NULL).
 * 
 * @param {TGenerique} cellule : cellule courante de la liste
 * @returns {TGenerique} : un pointeur vers la cellule suivante ou NULL
 * */
TGenerique suivant_processus(TGenerique cellule){
	if ( cellule == NULL ) return NULL;  
	return (TGenerique)((CelluleProcessus *)cellule)->suivant;
}


/**
 * Récupère le pointeur vers la cellule suivante
 * d'une cellule appartenant à la liste des résultats,
 * à condition que la cellule n'est pas vide (NULL).
 * 
 * @param {TGenerique} cellule : cellule courante de la liste
 * @returns {TGenerique} : un pointeur vers la cellule suivante ou NULL
 * */
TGenerique suivant_resultat(TGenerique cellule) {
	if ( cellule == NULL ) return NULL;  
	return (TGenerique)((CelluleResultat *)cellule)->suivant;
}



/**
 * Permet de renvoyer la dernier cellule d'une liste chainée quelconque.
 * Pour cela, elle parcourt toute la liste tantque le suivant de la cellule courante n'est pas vide (NULL)
 * et renvoie la cellule courant.
 * 
 * @param {TGenerique} l : une liste chainée quelconque
 * @param {FonctionSuivant} suivant : pointeur de fonction retournant un TGenerique associé au suivant de la liste chainée
 * 										(ex: suivant_processus, suivant_resultat)
 * 
 * @returns {TGenerique} l : la dernière cellule de la liste chainée
 * */
TGenerique queueListe(TGenerique l, FonctionSuivant suivant) {
	/* Renvoie le dernier element de la liste chainée */
	while ( !est_videListe(suivant(l)) ) {
		l = suivant(l);
	}
	return l;
}


/**
 * Permet de traiter la cellule courante d'une liste de processus.
 * Affiche le nom, le temps d'arrivée, le nombre de quantum associé au processus.
 * Si la cellule est vide, alors elle affiche un message de prévention. 
 * 
 * @param {TGenerique *} l : liste chainée à traiter
 * */
void traiterProcessus(TGenerique *l) {
	if ( est_videListe(*l) ) {
		printf("La ListeProcessus est vide\n");
		return;
	}
	printf("[PROCESSUS] Nom : %s | Arrivee : %d | Quantum : %d\n",
		processus_name((ListeProcessus)*l),
		time_arrival((ListeProcessus)*l),
		nb_quantum((ListeProcessus)*l)
	);
}


/**
 * Permet de traiter la cellule courante d'une liste de résultat.
 * Affiche la donnée associé au résultat.
 * Si la cellule est vide, alors elle affiche un message de prévention. 
 * 
 * @param {TGenerique *} l : liste chainée à traiter
 * */
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
/**
 * Récupère la chaine de caractères d'un processus,
 * dans la structure ListeProcessus.
 * 
 * @param {ListeProcessus} l : la liste des processus
 * @returns {char *} l->processus_name : la chaine de caractères du processus
 * */
char *processus_name(ListeProcessus l){	return l->processus_name; }


/**
 * Récupère le temps d'arrivée d'un processus,
 * dans la structure ListeProcessus.
 *  
 * @param {ListeProcessus} l : la liste des processus
 * @returns {int} l->time_arrival : le temps d'arrivée d'un processus
 * */
int time_arrival(ListeProcessus l){	return l->time_arrival; }


/**
 * Récupère le nombre de quantum d'un processus,
 * dans la structure ListeProcessus.
 * 
 * @param {ListeProcessus} l : la liste des processus
 * @returns {int} l->nb_quantum : le nombre de quantum du processus
 */
int nb_quantum(ListeProcessus l){ return l->nb_quantum; }



/******************************************
	Fonction primitive - ListeResultat
*******************************************/
/**
 * Récupère la donnée d'un résultat,
 * dans la structure ListeResultat.
 * 
 * @param {ListeResultat} l : la liste des résultats
 * @returns {int} l->donnee : la donnée du résultat
 */
int donnee(ListeResultat l){ return l->donnee; }



/******************************************
	Fonction d'insertion
*******************************************/
/**
 * Effectue l'insertion en tête dans la liste des processus.
 * Elle fait l'allocation mémoire de la nouvelle cellule,
 * remplit la structure avec de nouvelles données,
 * effectue le chainage entre la nouvelle cellule et la liste
 * 
 * @param {TGenerique *} l : liste de processus
 * @param {FonctionAlloc} alloc : pointeur de fonction retournant un TGenerique alloué
 * 									(ex : allocMem_processus, allocMem_resultat) * 
 * @param {const char *} name : la chaine de caractères du nouveau processus
 * @param {int} timeArrival : le temps d'arrivée du nouveau processus
 * @param {int} nbQuantum : le nombre de quantum du nouveau processus
 * */
void inserProcessusTete(TGenerique *l, FonctionAlloc alloc, const char *name, int timeArrival, int nbQuantum){
	CelluleProcessus *cellule = (CelluleProcessus *) alloc();

	if (cellule == NULL ) return;

	// remplissage des données
	strcpy(cellule->processus_name, name);	// copie une chaine de char
	cellule->time_arrival = timeArrival;
	cellule->nb_quantum = nbQuantum;
	cellule->suivant = (CelluleProcessus *) *l;
	*l = (TGenerique) cellule;
}


/**
 * Effectue l'insertion en tête dans la liste des résultats.
 * Elle fait l'allocation mémoire de la nouvelle cellule,
 * remplit la structure avec de nouvelles données,
 * effectue le chainage entre la nouvelle cellule et la liste
 * 
 * @param {TGenerique *} l : liste de résultat
 * @param {FonctionAlloc} alloc : pointeur de fonction retournant un TGenerique alloué
 * 									(ex : allocMem_processus, allocMem_resultat) * 
 * @param {int} donnee : la donnée du nouveau résultat
 * */
void inserResultatTete(TGenerique *l, FonctionAlloc alloc, int donnee){
	CelluleResultat *cellule = (CelluleResultat *) alloc();

	// remplissage des données
	cellule->donnee = donnee;
	cellule->suivant = (CelluleResultat *) *l;
	*l = (TGenerique) cellule;
}


/**
 * Effectue l'insertion en queue dans la liste des processus.
 * On vérifie si la cellule courante de la liste n'est pas vide (NULL),
 * si oui alors on fait l'insertion en tête,
 * sinon on récupère la dernière cellule avec la fonction "queueListe" pour ensuite
 * effectuer l'insertion en tête sur le suivant de la dernière cellule.
 * 
 * @param {TGenerique *} l : liste de processus
 * @param {FonctionAlloc} alloc : pointeur de fonction retournant un TGenerique alloué
 * 									(ex : allocMem_processus, allocMem_resultat) * 
 * @param {const char *} name : la chaine de caractères du nouveau processus
 * @param {int} timeArrival : le temps d'arrivée du nouveau processus
 * @param {int} nbQuantum : le nombre de quantum du nouveau processus
 * */
void inserProcessusQueue(TGenerique *l, FonctionAlloc alloc, const char *name, int timeArrival, int nbQuantum){
	if ( est_videListe(*l) ) {
		inserProcessusTete(l, alloc, name, timeArrival, nbQuantum);
	} else {
		CelluleProcessus *der = (CelluleProcessus *) queueListe(*l, suivant_processus); 
		inserProcessusTete((TGenerique *)&der->suivant, alloc, name, timeArrival, nbQuantum);
	}
}


/**
 * Effectue l'insertion en queue dans la liste des résultats.
 * On vérifie si la cellule courante de la liste n'est pas vide (NULL),
 * si oui alors on fait l'insertion en tête,
 * sinon on récupère la dernière cellule avec la fonction "queueListe" pour ensuite
 * effectuer l'insertion en tête sur le suivant de la dernière cellule.
 *  
 * @param {TGenerique *} l : liste de résultat
 * @param {FonctionAlloc} alloc : pointeur de fonction retournant un TGenerique alloué
 * 									(ex : allocMem_processus, allocMem_resultat) * 
 * @param {int} donnee : la donnée du nouveau résultat
 * */
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
/**
 * Supprime la cellule en tête d'une liste chaînée quelconque.
 * Si la liste est vide, elle ne fait rien.
 * 
 * @param {TGenerique *} l : liste chaînée quelconque
 * @param {FonctionSuivant} suivant : pointeur de fonction retournant le suivant
 *                                    (ex : suivant_processus, suivant_resultat)
 */
void suppTete(TGenerique *l, FonctionSuivant suivant){
	if ( est_videListe(*l) ) { return; }
	TGenerique tmp;
	tmp = *l;
	*l = suivant(*l);
	free(tmp);
}


/**
 * Supprime la dernière cellule de la liste des processus.
 * Si la liste est vide, elle ne fait rien.
 * Si la liste contient un seul élément, elle appelle suppTete.
 * Sinon, elle avance jusqu'à l'avant-dernier élément et libère le dernier.
 * 
 * @param {TGenerique *} l : liste de processus
 * @param {FonctionSuivant} suivant : pointeur de fonction retournant le suivant
 *                                    (ex : suivant_processus)
 */
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


/**
 * Supprime la dernière cellule de la liste des résultats.
 * Si la liste est vide, elle ne fait rien.
 * Si la liste contient un seul élément, elle appelle suppTete.
 * Sinon, elle avance jusqu'à l'avant-dernier élément et libère le dernier.
 * 
 * @param {TGenerique *} l : liste de résultats
 * @param {FonctionSuivant} suivant : pointeur de fonction retournant le suivant
 *                                    (ex : suivant_resultat)
 */
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



/******************************************
	Fonction de parcours
*******************************************/
void parcoursGD(TGenerique *l, FonctionSuivant suivant, FonctionTraiter traiter){
	TGenerique tmp = *l;
	while ( !est_videListe(tmp) ) {
		traiter(&tmp);
		tmp = suivant(tmp);
	} 
}