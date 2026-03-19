/**
 * @file sjf.c
 * @brief Implémentation de l'ordonnanceur Shortest Job First (SJF) non-préemptif.
 *
 * L'algorithme attend qu'un processus termine son bloc de calcul actuel
 * ou se bloque en E/S avant de ré-allouer le CPU au processus prêt le plus court.
 */


#include "sjf.h"


/**
 * @brief Point d'entrée principal de l'algorithme SJF.
 *
 * Pilote la simulation tick par tick. Gère les arrivées, le dispatching CPU (élection),
 * l'exécution des cycles UC/ES et la mise à jour de la chronologie d'exécution.
 *
 * @param liste  ListeTQ des processus à ordonnancer.
 * @param taille Nombre total de processus dans le système.
 * @return ExecutionTimeline* Pointeur vers l'historique complet de l'exécution.
 */
ExecutionTimeline* sjf(ListeTQ liste, int taille) {
	int temps_courant = 0;
	int nbProcessusTraiter = 0;

	ExecutionTimeline *resultat = allocTimeline();

	initTimelineProcessus(resultat, liste);
	ListeTQ listeTriee = trieListe(liste);

	File fileAttente = allocFile();
	File fileES = allocFile();

	ProcessusIterator *surLeCPU = NULL;
	Liste tete = teteLTQ(listeTriee);

	while (nbProcessusTraiter < taille) {

		// On traite les processus qui arrivent
		while (tete != NULL && (((Processus*)tete->data)->timeArrival == temps_courant)) {
			ProcessusIterator *it = (ProcessusIterator*) malloc(sizeof(ProcessusIterator));
			initIterator((Processus*)tete->data, it);
			enfilerFile(fileAttente, it);
			tete = suivantListe(tete);
		}

		// On avance les E/S, et on récupère ceux qui ont fini
		traiterES_SJF(&fileES, &fileAttente, resultat, &nbProcessusTraiter);


		// Choix du meilleur processus si CPU libre
		if (surLeCPU == NULL) {
			if (!estVideFile(fileAttente)) {
				surLeCPU = retirerMinTempsUC(&fileAttente);
			}
		}

		// Le processus sur le CPU travaille
		if (surLeCPU != NULL) {
			traiterUC(&surLeCPU, &fileES, resultat, &nbProcessusTraiter);
		}

		// Marquer le "Wait" pour ceux qui sont restés dans la file
		traiterWait(&fileAttente, resultat, temps_courant);

		temps_courant++;
	}

	// Libération mémoire
	destroyFile(fileAttente, free);
	destroyFile(fileES, free);
	destroyLTQ(listeTriee, NULL);

	if ( surLeCPU ) free(surLeCPU);
	return resultat;
}


/**
 * @brief Gère la progression des processus en phase d'Entrées/Sorties.
 *
 * Décrémente le temps restant en E/S. Si l'E/S est finie, le processus
 * est replacé dans la file d'attente pour sa prochaine rafale CPU.
 *
 * @param fileES             File des processus en ES.
 * @param fileAttente        File où replacer les processus redevenus prêts.
 * @param resultat           Chronologie à mettre à jour.
 * @param nbProcessusTraiter Compteur de processus terminés.
 */
void traiterES_SJF(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter) {
	if ( estVideFile(*fileES) ) return;


	File tmp = allocFile();
	if ( !tmp ) return;

	while ( !estVideFile(*fileES) ) {
		ProcessusIterator* it = (ProcessusIterator*) defilerFile(*fileES);
		avancerIterator(it);

		Processus *p = getTimelineProcessus(resultat, it);
		pushOrMergeOperationProcessus(p->listeTQ, ES, 1);
		// Si le temps ES est fini et qu'il a encore des temps de PCU
		if ( etatIterator(it) == UC ) {
			it->enAttente = true;   // ← signaler "vient d'arriver ce tick"
			enfilerFile(*fileAttente, it);
		} else if ( iteratorEstFini(it) ) {
			(*nbProcessusTraiter)++;
			free(it);
		} else {
			enfilerFile(tmp, it);
		}
	}
	destroyFile(*fileES, NULL);
	*fileES = tmp;
}