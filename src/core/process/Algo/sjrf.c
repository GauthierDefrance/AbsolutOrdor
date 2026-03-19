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

		// ARRIVÉES : On enfile les processus dont l'heure d'arrivée correspond au tick actuel
		while (tete != NULL && (((Processus*)tete->data)->timeArrival == temps_courant)) {
			ProcessusIterator *it = (ProcessusIterator*) malloc(sizeof(ProcessusIterator));
			initIterator((Processus*)tete->data, it);
			//it->enAttente = true;
			it->tempsEntreeFile = temps_courant;
			enfilerFile(fileAttente, it);
			tete = suivantListe(tete);
		}

		// ENTRÉES/SORTIES : On fait avancer les processus en ES
		traiterES_SJRF(&fileES, &fileAttente, resultat, &nbProcessusTraiter, temps_courant);

		// UNITÉ DE CALCUL : Le processus sur le CPU avance d'un tick
		if (surLeCPU != NULL) {
			traiterUC(&surLeCPU, &fileES, resultat, &nbProcessusTraiter);
		}

		// MÉCANISME DE PRÉEMPTION
		// On compare le processus actif avec le "meilleur" candidat prêt.
		// Si le candidat est plus court, on procède à un changement de contexte.
		if (surLeCPU != NULL && !estVideFile(fileAttente)) {
			ProcessusIterator* bestCandidat = retirerMinTempsUC(&fileAttente);  // ← SJRF
			if (bestCandidat != NULL && bestCandidat->tempsRestant < surLeCPU->tempsRestant) {
				surLeCPU->tempsEntreeFile = temps_courant;
				enfilerFile(fileAttente, surLeCPU);
				surLeCPU = bestCandidat;
			} else {
				if (bestCandidat != NULL) enfilerFile(fileAttente, bestCandidat);
			}
		}

		// ÉLECTION (si CPU libre) : Si personne n'est sur le CPU, on choisit le plus court
		if (surLeCPU == NULL && !estVideFile(fileAttente)) {
			surLeCPU = retirerMinTempsUC(&fileAttente);
			//printf("[SELECT t=%d] => %s restant=%d\n", temps_courant, surLeCPU->processus->name, surLeCPU->tempsRestant);
		}


		// ATTENTE : On enregistre le temps passé en file d'attente pour les processus "Prêts"
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