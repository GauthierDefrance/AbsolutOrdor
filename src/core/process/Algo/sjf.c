/**
* @file sjf.c
 * @brief Implémentation de l'ordonnanceur Shortest Job First (SJF) non-préemptif.
 * * Ce module simule un ordonnancement court terme prenant en charge les cycles
 * d'Unités de Calcul (UC), les Entrées/Sorties (ES) et les temps d'attente (Wait).
 * L'algorithme est non-préemptif : un processus occupant le CPU ne le libère
 * qu'à la fin de son bloc de calcul ou pour une opération d'ES.
 */


#include "sjf.h"


/* Prototypes des fonctions internes (statiques) */
static void traiterUC(ProcessusIterator** surLeCPU, const File *fileES, const ExecutionTimeline *resultat, int *nbProcessusTraiter);
static void traiterES(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter);
static void traiterWait(const File *fileAttente, const ExecutionTimeline *resultat, int temps_courant);


/**
 * @brief Point d'entrée principal de l'algorithme SJF.
 * Pilote la simulation tick par tick. Gère les arrivées, le dispatching CPU,
 * l'exécution des cycles UC/ES et la mise à jour de la chronologie d'exécution.
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
		traiterES(&fileES, &fileAttente, resultat, &nbProcessusTraiter);


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
 * @brief Gère l'exécution d'un tick d'Unité de Calcul sur le CPU.
 * @param surLeCPU Double pointeur vers le processus actif (permet la mise à NULL si fini).
 * @param fileES File où envoyer le processus s'il entame un cycle d'ES.
 * @param resultat Chronologie à mettre à jour.
 * @param nbProcessusTraiter Compteur de processus terminés à incrémenter.
 */
static void traiterUC(ProcessusIterator** surLeCPU, const File *fileES, const ExecutionTimeline *resultat, int *nbProcessusTraiter) {
	if (*surLeCPU == NULL) return;

	ProcessusIterator *it = *surLeCPU;
	Processus* p = getTimelineProcessus(resultat, it);

	// On avance (si le bloc finit, tempsRestant est mis à jour vers le bloc suivant)
	avancerIterator(it);

	// On marque l'UC dans la timeline
	pushOrMergeOperationProcessus(p->listeTQ, UC, 1);



	// VERIFICATION IMMEDIATE
	if (iteratorEstFini(it)) {
		(*nbProcessusTraiter)++;
		free(it);
		*surLeCPU = NULL;
	}
	else if (etatIterator(it) == ES) {
		// Si l'itérateur vient de passer en mode ES, on l'enlève du CPU tout de suite.
		enfilerFile(*fileES, it);
		*surLeCPU = NULL;
	}
}

/**
 * @brief Gère la progression des processus en phase d'Entrées/Sorties.
 * @param fileES File des processus en ES.
 * @param fileAttente File où replacer les processus redevenus prêts.
 * @param resultat Chronologie à mettre à jour.
 * @param nbProcessusTraiter Compteur de processus terminés.
 */
static void traiterES(File *fileES, const File *fileAttente, const ExecutionTimeline *resultat, int *nbProcessusTraiter) {
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


/**
 * @brief Enregistre le temps d'attente pour tous les processus en file d'attente.
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

		// Arrivé strictement avant ce tick = il attend ce tick
		if (p->timeArrival <= temps_courant && !it->enAttente) {
			pushOrMergeOperationProcessus(p->listeTQ, W, 1);
		}
		it->enAttente = false;
		enfilerFile(tmp, it);
	}
	while (!estVideFile(tmp)) enfilerFile(*fileAttente, defilerFile(tmp));
	destroyFile(tmp, NULL);
}