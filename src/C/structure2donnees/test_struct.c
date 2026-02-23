#include "structure_processeur.h"
#include <assert.h>


/**
 * Vérifie que l'allocation mémoire d'une liste chaînée a réussi,
 * c'est-à-dire que la liste n'est pas NULL après l'allocation.
 * 
 * @param {TGenerique *} l : liste chaînée à vérifier
 * */
void test_allocationMem(TGenerique *l){
	assert(!est_videListe(*l));
	printf("Assertion reussi - Liste bien allouer\n");
}


/**
 * Effectue la libération mémoire d'une liste chaînée
 * et vérifie que la liste vaut NULL après la libération.
 * 
 * @param {TGenerique *} l : liste chaînée à libérer
 * @param {FonctionSuivant} suivant : pointeur de fonction retournant le suivant
 *                                    (ex : suivant_processus, suivant_resultat)
 * */
void test_liberationMem(TGenerique *l, FonctionSuivant suivant){
	libListeMem(l, suivant);
	assert(est_videListe(*l));
	printf("Assertion reussi - Liste bien liberer\n");
}


/**
 * Vérifie que l'initialisation d'une liste chaînée a réussi,
 * c'est-à-dire que la liste vaut NULL après l'initialisation.
 * 
 * @param {TGenerique *} l : liste chaînée à vérifier
 * */
void test_initialisation(TGenerique *l){
	assert(est_videListe(*l));
	printf("Assertion reussi - Liste bien initialiser\n");
}


/**
 * Vérifie que l'insertion en tête a réussi,
 * c'est-à-dire que la liste n'est pas vide après l'insertion.
 * 
 * @param {TGenerique *} l : liste chaînée à vérifier
 * */
void test_inserTete(TGenerique *l){
	assert(!est_videListe(*l));
	printf("Assertion reussi - Insertion en Tete effectuee\n");
}


/**
 * Vérifie que l'insertion en queue a réussi,
 * c'est-à-dire que la liste n'est pas vide après l'insertion.
 * 
 * @param {TGenerique *} l : liste chaînée à vérifier
 * */
void test_inserQueue(TGenerique *l){
	assert(!est_videListe(*l));
	printf("Assertion reussi - Insertion en Queue effectuee\n");
}


/**
 * Vérifie que la suppression en tête a réussi.
 * Contrôle que la tête a changé et que l'ancienne tête existait bien.
 * 
 * @param {TGenerique *} l : liste chaînée à vérifier
 * @param {TGenerique} ancienneTete : pointeur vers l'ancienne tête avant suppression
 * */
void test_suppTete(TGenerique *l, TGenerique ancienneTete){
	assert( ancienneTete != *l );			// la tête à changer
	assert( ancienneTete != NULL );			// l'ancienne tête existait bel et bien
	printf("Assertion reussi - Suppression en Tete effectuee\n");
}


/**
 * Vérifie que la suppression en queue a réussi.
 * Contrôle que la queue a changé et que l'ancienne queue existait bien.
 * 
 * @param {TGenerique *} l : liste chaînée à vérifier
 * @param {TGenerique} ancienneQueue : pointeur vers l'ancienne queue avant suppression
 */
void test_suppQueue(TGenerique *l, TGenerique ancienneQueue){
	assert(ancienneQueue != *l);			// la queue à changer
	assert( ancienneQueue != NULL );		// l'ancienne queue existait bel et bien
	printf("Assertion reussi - Suppression en Queue effectuee\n");
}


/******************************************
	main
*******************************************/
void main(void) {

	printf("\nDebogage\n");

	/******************************************
		TEST - Allocation mémoire
	*******************************************/
	printf("\n\n=== TEST d'allocation memoire ===\n");


	// vrai liste
	ListeProcessus ListeP;
	ListeResultat ListeR;


	// allocation memoire
	ListeP = (ListeProcessus) allocMem_processus();
	test_allocationMem((TGenerique *)&ListeP);

	ListeR = (ListeResultat) allocMem_resultat();
	test_allocationMem((TGenerique *)&ListeR);



	/******************************************
		TEST - Initialisation des listes
	*******************************************/
	printf("\n\n=== TEST initialisation ===\n");


	// initialisation
	ListeP = (ListeProcessus)initListe();
	ListeR = (ListeResultat)initListe();

	test_initialisation((TGenerique *)&ListeP);
	test_initialisation((TGenerique *)&ListeR);

	// fausse liste pour les parcours et tout le blabla
	ListeProcessus tmp1 = ListeP;
	ListeResultat tmp2 = ListeR;



	/******************************************
		TEST - Insertion Tête
	*******************************************/
	printf("\n\n=== TEST inserTete ===\n");


	// insertion en Tete ListeProcessus
	inserProcessusTete((TGenerique *)&ListeP, allocMem_processus, "processus1", 5, 3);
	test_inserTete((TGenerique *)&ListeP);

	inserProcessusTete((TGenerique *)&ListeP, allocMem_processus, "processus2", 0, 2);
	test_inserTete((TGenerique *)&ListeP);


	// insertion en Tete ListeResultat
	inserResultatTete((TGenerique *)&ListeR, allocMem_resultat, 1);
	test_inserTete((TGenerique *)&ListeR);

	inserResultatTete((TGenerique *)&ListeR, allocMem_resultat, 2);
	test_inserTete((TGenerique *)&ListeR);



	/******************************************
		TEST - Insertion Queue
	*******************************************/
	printf("\n\n=== TEST inserQueue ===\n");


	// insertion en Queue de ListeProcessus et ListeResultat
	inserProcessusQueue((TGenerique *)&ListeP, allocMem_processus, "processus0", 1, 5);
	test_inserQueue((TGenerique *)&ListeP);

	inserResultatQueue((TGenerique *)&ListeR, allocMem_resultat, 0);
	test_inserQueue((TGenerique *)&ListeR);



	/******************************************
		TEST - Affichage
	*******************************************/
	printf("\n\n=== TEST affichage ===\n");


	parcoursGD((TGenerique *)&ListeP, suivant_processus, traiterProcessus);
	parcoursGD((TGenerique *)&ListeR, suivant_resultat,  traiterResultat);



	/******************************************
		TEST - Suppression Tête
	*******************************************/
	printf("\n\n=== TEST suppTete ===\n");


	// suppression en tête de ListeProcessus et ListeResultat
	tmp1 = ListeP;
	tmp2 = ListeR;

	suppTete((TGenerique *)&ListeP, suivant_processus);
	test_suppTete((TGenerique *)&ListeP, tmp1);

	suppTete((TGenerique *)&ListeR, suivant_resultat);
	test_suppTete((TGenerique *)&ListeR, tmp2);



	/******************************************
		TEST - Suppression Queue
	*******************************************/
	printf("\n\n=== TEST suppQueue ===\n");

	tmp1 = queueListe((TGenerique *)ListeP, suivant_processus);
	tmp2 = queueListe((TGenerique *)ListeR, suivant_resultat);

	suppProcessusQueue((TGenerique *)&ListeP, suivant_processus);
	test_suppQueue((TGenerique *)&ListeP, tmp1);

	suppResultatQueue((TGenerique *)&ListeR, suivant_resultat);
	test_suppQueue((TGenerique *)&ListeR, tmp2);



	/******************************************
		TEST - Affichage
	*******************************************/
	printf("\n\n=== TEST affichage ===\n");


	parcoursGD((TGenerique *)&ListeP, suivant_processus, traiterProcessus);
	parcoursGD((TGenerique *)&ListeR, suivant_resultat,  traiterResultat);



	/******************************************
		TEST - Libération mémoire
	*******************************************/
	printf("\n\n=== TEST liberation memoire ===\n");

	test_liberationMem((TGenerique *)&ListeP, suivant_processus);
	test_liberationMem((TGenerique *)&ListeR, suivant_resultat);


	printf("\n\tFin debogage\n\n");
}