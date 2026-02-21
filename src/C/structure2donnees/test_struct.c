#include "structure_processeur.h"


/******************************************
	main
*******************************************/
void main(void) {
	printf("\n\tBonjour, debut du debogage !!!\n\n");
	// vrai liste
	ListeProcessus ListeP;
	ListeResultat ListeR;

	// fausse liste pour les parcours et tout le blabla
	ListeProcessus tmp1 = ListeP;
	ListeResultat tmp2 = ListeR;


	// allocation memoire
	ListeP = (ListeProcessus)allocMem(allocMem_processus);
	ListeR = (ListeResultat)allocMem(allocMem_resultat);

	// initialisation
	ListeP = (ListeProcessus)initListe();
	ListeR = (ListeResultat)initListe();


	// Traiter tout les données de la liste avant les insertion
	printf("ListeProcessus vide ? %s\n" ,est_videListe(ListeP) ? "Vrai" : "Faux");
	printf("ListeResultat vide ? %s\n" ,est_videListe(ListeP) ? "Vrai" : "Faux");


	// insertion en Tete ListeProcessus
	inserProcessusTete((TGenerique *)&ListeP, allocMem_processus, "processus1", 5, 3);
	inserProcessusTete((TGenerique *)&ListeP, allocMem_processus, "processus2", 0, 2);


	// insertion en Tete ListeResultat
	inserResultatTete((TGenerique *)&ListeR, allocMem_resultat, 1);
	inserResultatTete((TGenerique *)&ListeR, allocMem_resultat, 2);


	// Traiter tout les données de la liste  après insertTete
	printf("\n\tTraiter tout les donnees de la liste  apres insertTete\n");
	tmp1 = ListeP;
	while ( !est_videListe(tmp1) ) {
		traiterProcessus((TGenerique *)&tmp1);
		tmp1 = tmp1->suivant;
	} 
	tmp2 = ListeR;
	while ( !est_videListe(tmp2) ) {
		traiterResultat((TGenerique *)&tmp2);
		tmp2 = tmp2->suivant;
	}


	// Traiter le dernier
	printf("\n\tTraiter le dernier de chaque liste\n");
	printf("dernier de ListeProcessus =>");
	tmp1 = ListeP;
	TGenerique dernierP = queueListe((TGenerique)tmp1, suivant_processus);
	traiterProcessus(&dernierP);
	printf("dernier de ListeResultat =>");
	tmp2 = ListeR;
	TGenerique dernierR = queueListe((TGenerique)tmp2, suivant_resultat);
	traiterResultat(&dernierR);




	// insertion en Queue de ListeProcessus et ListeResultat
	inserProcessusQueue((TGenerique *)&ListeP, allocMem_processus, "processus0", 1, 5);
	inserResultatQueue((TGenerique *)&ListeR, allocMem_resultat, 0);


	// Traiter tout les données de la liste après insertQueue
	printf("\n\tTraiter tout les donnees de la liste apres insertQueue\n");
	tmp1 = ListeP;
	while ( !est_videListe(tmp1) ) {
		traiterProcessus((TGenerique *)&tmp1);
		tmp1 = tmp1->suivant;
	} 
	tmp2 = ListeR;
	while ( !est_videListe(tmp2) ) {
		traiterResultat((TGenerique *)&tmp2);
		tmp2 = tmp2->suivant;
	}


	// suppression en tête de ListeProcessus et ListeResultat
	printf("\n\tsuppression en tete de ListeProcessus et ListeResultat\n");
	suppTete((TGenerique)&ListeP, suivant_processus);
	suppTete((TGenerique)&ListeR, suivant_resultat);


	// Traiter tout les données de la liste après la suppression en Tête
	printf("\n\tTraiter tout les donnees de la liste apres la suppression en Tete\n");
	tmp1 = ListeP;
	while ( !est_videListe(tmp1) ) {
		traiterProcessus((TGenerique *)&tmp1);
		tmp1 = tmp1->suivant;
	} 
	tmp2 = ListeR;
	while ( !est_videListe(tmp2) ) {
		traiterResultat((TGenerique *)&tmp2);
		tmp2 = tmp2->suivant;
	}


	// suppression en queue de ListeProcessus et ListeResultat
	printf("\n\tsuppression en queue de ListeProcessus et ListeResultat\n");
	suppProcessusQueue((TGenerique)&ListeP, suivant_processus);
	suppResultatQueue((TGenerique)&ListeR, suivant_resultat);


	// Traiter tout les données de la liste après la suppression en Tête
	printf("\n\tTraiter tout les donnees de la liste apres la suppression en queue\n");
	tmp1 = ListeP;
	while ( !est_videListe(tmp1) ) {
		traiterProcessus((TGenerique *)&tmp1);
		tmp1 = tmp1->suivant;
	} 
	tmp2 = ListeR;
	while ( !est_videListe(tmp2) ) {
		traiterResultat((TGenerique *)&tmp2);
		tmp2 = tmp2->suivant;
	}


	// libération mémoire
	libListeMem((TGenerique *)&ListeP, suivant_processus);
	libListeMem((TGenerique *)&ListeR, suivant_resultat);
	printf("\n\tFin du debogage, au revoir !!!\n\n");
}