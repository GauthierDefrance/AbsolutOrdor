/**
 * @file sjrf.c
 * @brief Implémentation de l'ordonnanceur Shortest Job Remaining First (SJRF).
 *
 * Le SJRF est la variante préemptive du SJF (Shortest Job First). À chaque tick d'horloge,
 * l'algorithme évalue si un nouveau processus arrivé ou sortant d'E/S possède un temps
 * d'exécution restant inférieur à celui du processus en cours.
 * Si c'est le cas, le processus actuel est préempté et replacé en file d'attente.
 */


#include "sjrf.h"


/**
 * @brief Algorithme SJRF : Pilote la simulation tick par tick.
 *
 * Cette fonction gère le cycle de vie complet : arrivées, exécution CPU (UC),
 * attentes en file d'E/S et mécanisme de préemption basé sur le temps restant.
 *
 * @param liste  ListeTQ des processus à ordonnancer.
 * @param taille Nombre total de processus (condition d'arrêt).
 * @return ExecutionTimeline* Historique complet des états pour chaque tick.
 */
ExecutionTimeline* sjrf(ListeTQ liste, int taille) {
	int temps_courant = 0;
	int nbProcessusTraiter = 0;

	// Initialisation de la structure de résultats
	ExecutionTimeline *resultat = allocTimeline();
	initTimelineProcessus(resultat, liste);

	// Tri initial par temps d'arrivée pour faciliter la gestion de la file d'entrée
	ListeTQ listeTriee = trieListe(liste);

	// Files de gestion des états
	File fileAttente = allocFile();
	File fileES = allocFile();

	ProcessusIterator *surLeCPU = NULL;
	Liste tete = teteLTQ(listeTriee);

	// Boucle de simulation principale
	while (nbProcessusTraiter < taille) {
		// 1. ARRIVÉES
		while (tete != NULL && (((Processus*)tete->data)->timeArrival == temps_courant)) {
			ProcessusIterator *it = malloc(sizeof(ProcessusIterator));
			initIterator((Processus*)tete->data, it);
			it->tempsEntreeFile = temps_courant;
			enfilerFile(fileAttente, it);
			tete = suivantListe(tete);
		}

		// 2. ENTRÉES/SORTIES (Ceux qui finissent vont en fileAttente)
		traiterES_SJRF(&fileES, &fileAttente, resultat, &nbProcessusTraiter, temps_courant);

		// 3. EXÉCUTION CPU (On fait travailler celui qui était là)
		if (surLeCPU != NULL) {
			traiterUC(&surLeCPU, &fileES, resultat, &nbProcessusTraiter);
			// Si traiterUC a fini le bloc, surLeCPU est maintenant NULL
		}

		// 4. PRÉEMPTION / ÉLECTION (On décide qui sera sur le CPU au prochain tick)
		if (surLeCPU != NULL) {
			if (!estVideFile(fileAttente)) {
				ProcessusIterator* best = retirerMinTempsUC(&fileAttente);
				if (best->tempsRestant < surLeCPU->tempsRestant) {
					printf("[TICK %d] PREEMPTION: %s prend la place de %s\n", temps_courant, best->processus->name, surLeCPU->processus->name);
					enfilerFile(fileAttente, surLeCPU);
					surLeCPU = best;
				} else {
					enfilerFile(fileAttente, best);
				}
			}
		} else if (!estVideFile(fileAttente)) {
			surLeCPU = retirerMinTempsUC(&fileAttente);
			printf("[TICK %d] ELECTION: %s\n", temps_courant, surLeCPU->processus->name);
		}

		// 5. COMPTER L'ATTENTE (Tous ceux qui n'ont pas été élus)
		traiterWait_SJFR(&fileAttente, resultat, temps_courant);

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
 * @brief Gère l'avancement des processus en phase d'Entrées/Sorties.
 *
 * Décrémente le temps restant en E/S pour chaque processus bloqué.
 * Si une E/S se termine, le processus est replacé dans la file d'attente (Prêt).
 *
 * @param fileES Pointeur vers la file des processus en E/S.
 * @param fileAttente File des processus prêts (pour le retour d'E/S).
 * @param resultat Structure de chronologie à mettre à jour.
 * @param nbProcessusTraiter Compteur incrémenté si un processus se termine en E/S.
 * @param temps_courant Horloge actuelle de la simulation.
 */
void traiterES_SJRF(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter, int temps_courant) {
	if ( estVideFile(*fileES) ) return;

	// File temporaire pour reconstruire la file ES
	File tmp = allocFile();
	if ( !tmp ) return;

	while ( !estVideFile(*fileES) ) {
		ProcessusIterator* it = (ProcessusIterator*) defilerFile(*fileES);
		//printf("[ES t=?] %s restant=%d\n", it->processus->name, it->tempsRestant);
		avancerIterator(it);

		Processus *p = getTimelineProcessus(resultat, it);
		pushOrMergeOperationProcessus(p->listeTQ, ES, 1);

		if ( etatIterator(it) == UC ) {
			// L'E/S est terminée, le processus redevient "Prêt" (UC suivant)
			it->tempsEntreeFile = temps_courant - 1;  // tick précédent
			it->enAttente = true;
			enfilerFile(*fileAttente, it);
		} else if ( iteratorEstFini(it) ) {
			(*nbProcessusTraiter)++;
			free(it);
		} else {
			// L'E/S n'est pas encore finie
			enfilerFile(tmp, it);
		}
	}
	destroyFile(*fileES, NULL);
	*fileES = tmp;
}


/**
 * @brief Enregistre une unité de temps d'attente pour les processus en file.
 *
 * Parcourt l'ensemble de la file d'attente (Ready Queue) et incrémente l'état
 * "Wait" (W) dans la chronologie pour chaque processus dont l'heure d'arrivée
 * est inférieure ou égale au temps actuel. Cette fonction est appelée après
 * les décisions d'élection et de préemption pour garantir la précision du diagramme.
 *
 * @param fileAttente   Pointeur vers la file des processus prêts à être exécutés.
 * @param resultat      Pointeur vers la structure globale de suivi de l'exécution.
 * @param temps_courant Horloge actuelle de la simulation (tick).
 */
void traiterWait_SJFR(const File *fileAttente, const ExecutionTimeline *resultat, int temps_courant) {
	if (estVideFile(*fileAttente)) return;

	File tmp = allocFile();
	while (!estVideFile(*fileAttente)) {
		ProcessusIterator* it = (ProcessusIterator*) defilerFile(*fileAttente);
		Processus *p = getTimelineProcessus(resultat, it);

		// On enregistre un Wait (W) pour TOUS ceux qui sont en file
		// condition p->timeArrival <= temps_courant est une sécurité
		if (it->processus->timeArrival <= temps_courant) {
			pushOrMergeOperationProcessus(p->listeTQ, W, 1);
		}

		enfilerFile(tmp, it);
	}

	// Restauration de la file
	while (!estVideFile(tmp)) enfilerFile(*fileAttente, defilerFile(tmp));
	destroyFile(tmp, NULL);
}