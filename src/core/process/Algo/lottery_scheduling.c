/**
 * @file lottery_scheduling.c
 * @brief Implémentation de l'ordonnancement par loterie (Lottery Scheduling) en mode rafale.
 *
 * Cet algorithme utilise une approche probabiliste où chaque processus en file d'attente
 * possède un nombre de tickets proportionnel à son temps d'exécution restant (UC).
 * Le mode "rafale" garantit qu'une fois un processus élu, il conserve le CPU jusqu'à
 * la fin de son bloc de calcul ou son passage en E/S (non-préemptif).
 */
#include "lottery_scheduling.h"


/**
 * @brief Simule l'ordonnancement des processus selon l'algorithme de la loterie.
 *
 * @param liste La liste des processus à ordonnancer (chargée depuis CSV).
 * @param taille Le nombre total de processus dans la simulation.
 * @return ExecutionTimeline* La structure contenant l'historique de l'exécution (Gantt).
 */
ExecutionTimeline* lottery_scheduling(ListeTQ liste, int taille) {
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
		// ARRIVÉES
		while (tete != NULL && (((Processus*)tete->data)->timeArrival == temps_courant)) {
			ProcessusIterator *it = malloc(sizeof(ProcessusIterator));
			initIterator((Processus*)tete->data, it);
			it->tempsEntreeFile = temps_courant;
			enfilerFile(fileAttente, it);
			tete = suivantListe(tete);
		}

		// ENTRÉES/SORTIES
		traiterES_SJRF(&fileES, &fileAttente, resultat, &nbProcessusTraiter, temps_courant);

		// EXÉCUTION CPU
		if (surLeCPU != NULL) {
			traiterUC(&surLeCPU, &fileES, resultat, &nbProcessusTraiter);
			// Si traiterUC a fini le bloc, surLeCPU est maintenant NULL
		}

		// ÉLECTION
	    // On ne préempte PAS. On attend que le CPU soit libre !!!
		if (surLeCPU == NULL && !estVideFile(fileAttente) ) {
            surLeCPU = tirageGagnant(&fileAttente);
		}

		// ATTENTE
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
 * @brief Sélectionne un processus par tirage au sort (Lottery Scheduling).
 *
 * Le nombre de tickets d'un processus est égal à son temps d'exécution restant
 * dans son quantum actuel (tempsRestant). Plus un processus a de travail à faire,
 * plus il a de chances d'être élu.
 *
 * @param f Pointeur vers la file des processus prêts (Ready Queue).
 * @return ProcessusIterator* Le gagnant extrait de la file, ou NULL si file vide.
 */
ProcessusIterator* tirageGagnant(const File *f) {
    if ( estVideFile(*f) ) return NULL;

    // Compter le total des tickets
    int totalTickets = 0;
    File tmp = allocFile();

    while ( !estVideFile(*f) ) {
        ProcessusIterator* it = (ProcessusIterator*) defilerFile(*f);
        totalTickets += it->tempsRestant;        // Nb de ticket total = nombre de quantum restant pour tout les processus en file d'attente
        enfilerFile(tmp, it);
    }

    // Tirage au sort
    int ticketGagnants = rand() % totalTickets;
    int compteur = 0;
    ProcessusIterator* processus_gagnant = NULL;

    // Trouver le gagnant
    while ( !estVideFile(tmp) ) {
        ProcessusIterator* it = (ProcessusIterator*) defilerFile(tmp);
        compteur += it->tempsRestant;

        if ( processus_gagnant == NULL && compteur > ticketGagnants ) {
            processus_gagnant = it;
        } else {
            enfilerFile(*f,it);
        }
    }

    destroyFile(tmp, NULL);
    return processus_gagnant;
}