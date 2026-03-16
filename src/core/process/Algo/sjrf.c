/**
 * @file sjrf.c
 * @brief Implémentation de l'ordonnanceur Shortest Job Remaining First (SJRF).
 * Le SJRF est la variante préemptive du SJF. À chaque tick d'horloge, l'algorithme
 * vérifie si un processus avec un temps d'exécution restant plus court est prêt.
 * Si c'est le cas, le processus actuel est interrompu (préempté).
 */


#include "sjrf.h"


/* --- Prototypes des fonctions internes --- */
static void traiterUC(ProcessusIterator** surLeCPU, const File *fileES, const ExecutionTimeline *resultat, int *nbProcessusTraiter);
static void traiterES(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter, int temps_courant);
static void traiterWait(const File *fileAttente, const ExecutionTimeline *resultat, int temps_courant);


/**
 * @brief Algorithme SJRF : Pilote la simulation tick par tick.
 * Pilote la simulation tick par tick. Gère les arrivées, le dispatching CPU,
 * l'exécution des cycles UC/ES et la mise à jour de la chronologie d'exécution.
 * @param liste  ListeTQ des processus à ordonnancer.
 * @param taille Nombre total de processus dans le système.
 * @return ExecutionTimeline* Pointeur vers l'historique complet de l'exécution.
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
		traiterES(&fileES, &fileAttente, resultat, &nbProcessusTraiter, temps_courant);

		// UNITÉ DE CALCUL : Le processus sur le CPU avance d'un tick
		if (surLeCPU != NULL) {
			traiterUC(&surLeCPU, &fileES, resultat, &nbProcessusTraiter);
		}

		// MÉCANISME DE PRÉEMPTION
		// Si le CPU est occupé, on regarde si un processus plus court vient d'arriver en file d'attente
		// MÉCANISME DE PRÉEMPTION
		if (surLeCPU != NULL && !estVideFile(fileAttente)) {
			ProcessusIterator* bestCandidat = retirerMinTempsUCSJRF(&fileAttente);  // ← SJRF
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
			surLeCPU = retirerMinTempsUCSJRF(&fileAttente);
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
 * @brief Exécute une unité de temps sur le processeur.
 * @param surLeCPU Double pointeur vers le processus actif (permet la mise à NULL si fini).
 * @param fileES File où envoyer le processus s'il entame un cycle d'ES.
 * @param resultat Chronologie à mettre à jour.
 * @param nbProcessusTraiter Compteur de processus terminés à incrémenter.
 */
static void traiterUC(ProcessusIterator** surLeCPU, const File *fileES, const ExecutionTimeline *resultat, int *nbProcessusTraiter) {
	if (*surLeCPU == NULL) return;

	ProcessusIterator *it = *surLeCPU;
	Processus* p = getTimelineProcessus(resultat, it);

	// Décrémente le temps restant du bloc CPU actuel
	avancerIterator(it);

	// Enregistre l'activité CPU dans la chronologie (fusionne si bloc continu)
	pushOrMergeOperationProcessus(p->listeTQ, UC, 1);

	// Vérification du changement d'état
	if (iteratorEstFini(it)) {
		(*nbProcessusTraiter)++;
		free(it);
		*surLeCPU = NULL;
	}
	else if (etatIterator(it) == ES) {
		// Le bloc UC est fini, le processus doit maintenant faire une E/S
		it->enAttente = true;
		enfilerFile(*fileES, it);
		*surLeCPU = NULL;
	}
}

/**
 * @brief Gère l'avancement des processus effectuant des Entrées/Sorties.
 * @param fileES File des processus en ES.
 * @param fileAttente File où replacer les processus redevenus prêts.
 * @param resultat Chronologie à mettre à jour.
 * @param nbProcessusTraiter Compteur de processus terminés.
 * @param temps_courant Tick courant de la simulation
 */
static void traiterES(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter, int temps_courant) {
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
 * @brief Enregistre les statistiques d'attente pour la Ready Queue.
 * @param fileAttente File des processus prêts (Ready Queue).
 * @param resultat Chronologie à mettre à jour.
 * @param temps_courant Tick courant de la simulation
 */
static void traiterWait(const File *fileAttente, const ExecutionTimeline *resultat, int temps_courant) {
	if (estVideFile(*fileAttente)) return;

	File tmp = allocFile();
	while (!estVideFile(*fileAttente)) {
		ProcessusIterator* it = (ProcessusIterator*) defilerFile(*fileAttente);
		Processus *p = getTimelineProcessus(resultat, it);

		// Si le processus est en file et n'est pas marqué "en cours de transition"
		if (p->timeArrival <= temps_courant && !it->enAttente) {
			pushOrMergeOperationProcessus(p->listeTQ, W, 1);
		}

		// Gestion du flag de transition pour éviter de compter l'attente au tick d'arrivée exact
		if (it->tempsEntreeFile < temps_courant) {
			it->enAttente = false;
		}
		enfilerFile(tmp, it);
	}

	// Restauration de la file
	while (!estVideFile(tmp)) enfilerFile(*fileAttente, defilerFile(tmp));
	destroyFile(tmp, NULL);
}

/**
 * @brief Recherche et extrait le processus avec le temps de calcul restant le plus court.
 * Critère : tempsRestant MIN, puis temps d'arrivée MIN en cas d'égalité.
 */
ProcessusIterator* retirerMinTempsUCSJRF(const File* f) {
	if (*f == NULL || estVideFile(*f)) return NULL;

	File tmp = allocFile();
	ProcessusIterator* min = NULL;
	int n = tailleFile(*f);

	for (int i = 0; i < n; i++) {
		ProcessusIterator* p = (ProcessusIterator*) defilerFile(*f);

		if (min == NULL || p->tempsRestant < min->tempsRestant || (p->tempsRestant == min->tempsRestant && p->tempsEntreeFile < min->tempsEntreeFile)) {

			if (min != NULL) enfilerFile(tmp, min);
			min = p;
		} else {
			enfilerFile(tmp, p);
		}
	}

	// Remettre les autres processus dans la file d'origine
	while (!estVideFile(tmp))
		enfilerFile(*f, defilerFile(tmp));
	destroyFile(tmp, NULL);
	return min;
}