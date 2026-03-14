#include "sjf.h"

// ALGO NON-PRÉEMPTIF


static void traiterUC(ProcessusIterator** surLeCPU,File *fileAttente, File *fileES, ExecutionTimeline *resultat, int *nbProcessusTraiter);
static void traiterES(File *fileES, File *fileAttente, ExecutionTimeline *resultat, int *nbProcessusTraiter);
static void traiterWait(File *fileAttente, ExecutionTimeline *resultat);

/**
 * @param : listeTQ de processus
 * @return : listeTQ de processus ordonnée selon SJF
 * */
ExecutionTimeline* sjf(ListeTQ liste, int taille) {
	int temps_courant = 0;					// Temps courant
	int nbProcessusTraiter = 0;


	// Liste d'ordonnancement SJF en sortie
	ExecutionTimeline *resultat = allocTimeline();
	initTimelineProcessus(resultat, liste);


	// Crée un liste dynamique de processus ranger par temps d'arrivé
	ListeTQ listeTriee = trieListe(liste);


	// Crée les File nécessaire au traitement SJF
	File fileAttente = allocFile();				// File d'attent du CPU qui accumule W
	File fileES = allocFile();					// ES en cours en parallèle
	ProcessusIterator *surLeCPU = NULL;			// Processus en cours d'exécution sur le CPU

	// Vérifiaction des structure
	if ( !resultat || !fileAttente || !fileES || !listeTriee ) return NULL;
	initFile(fileAttente);						// Contient des ProcessusIterator
	initFile(fileES);							// Contient des ProcessusIterator



	Liste tete = teteLTQ(listeTriee);


	// Ajoute dans une File les processus à traiter 
	while ( nbProcessusTraiter < taille ) {
    	//printf("=== tick %d | traités: %d/%d ===\n", temps_courant, nbProcessusTraiter, taille);
		/**
		 * Ajout des processus a traiter dans la file d'attente
		 * auquel leur temps d'arrivée correspond au temps courant
		 * */
		while ( tete != NULL && ( ((Processus*)tete->data)->timeArrival == temps_courant ) ) {
	        //printf("  -> %s arrive dans fileAttente\n", ((Processus*)tete->data)->name);
			ProcessusIterator *it = malloc(sizeof(ProcessusIterator));
			initIterator((Processus*)tete->data,it);
			enfilerFile(fileAttente, it); 
			tete = suivantListe(tete);
		}

    	//printf("  surLeCPU: %s\n", surLeCPU ? surLeCPU->processus->name : "NULL");

		
		//
		traiterES(&fileES, &fileAttente, resultat, &nbProcessusTraiter);

    	//printf("  apres traiterUC: surLeCPU=%s | nbTraites=%d\n", surLeCPU ? surLeCPU->processus->name : "NULL", nbProcessusTraiter);
		// Avancer tous les processus dans la fileES d'un tick
		traiterUC(&surLeCPU, &fileAttente, &fileES, resultat, &nbProcessusTraiter);


		// Faire accumuler W à tout les processus dans la fileAttente
		traiterWait(&fileAttente, resultat);
		// 3. Log minimaliste et efficace
	    /*if (temps_courant % 10 == 0) { // Log tous les 10 ticks pour ne pas inonder la console
	        printf("Tick %d : Traités %d/%d | CPU: %s\n", 
	                temps_courant, nbProcessusTraiter, taille, 
	                surLeCPU ? surLeCPU->processus->name : "IDLE");
	    }*/

		temps_courant ++;
	    /*if (temps_courant > 2000) {
	        //printf("TIMEOUT - boucle infinie detectee\n");
	        break;
	    }*/


	}

	// libération mémoire des structure intermédiaire
	destroyFile(fileAttente, free);
	destroyFile(fileES, free);
	destroyLTQ(listeTriee, NULL);
	
	if ( surLeCPU ) free(surLeCPU);
	afficherResultatsSJF(resultat);
	afficherDiagramme(resultat);
	return resultat;
}


/**
 * Tri les processus selon leur temps d'arrivée.
 * @param ltq : la liste source non triée
 * @return : une nouvelle liste chaînée triée
 */
ListeTQ trieListe(ListeTQ ltq) {
    if (!ltq || teteLTQ(ltq) == NULL) return NULL;

    ListeTQ resultat = allocMemLTQ();
    ListeTQ copieTravail = allocMemLTQ();
    
    // 1. On recopie les pointeurs des processus dans une liste temporaire
    Liste courant = teteLTQ(ltq);
    while (courant != NULL) {
        inserQueueLTQ(copieTravail, courant->data);
        courant = suivantListe(courant);
    }

    // 2. On extrait itérativement le processus avec le temps d'arrivée min
    while (teteLTQ(copieTravail) != NULL) {
        Liste minNode = teteLTQ(copieTravail);
        Processus* minProc = (Processus*)minNode->data;
        Liste it = minNode->suivant;

        // Chercher le plus petit temps d'arrivée
        while (it != NULL) {
            Processus* p = (Processus*)it->data;
            if (p->timeArrival < minProc->timeArrival) {
                minProc = p;
                minNode = it;
            }
            it = suivantListe(it);
        }

        // Ajouter au résultat et supprimer de la copie sans détruire le processus
        inserQueueLTQ(resultat, minProc);
        supprimerNoeudLTQ(copieTravail, minNode, NULL); 
    }

    // 3. Libération de la liste temporaire (les cellules seulement)
    destroyLTQ(copieTravail, NULL); 
    return resultat;
}


/**
 * 
 * */
ProcessusIterator* estMinTempsUCProcessus(File* f){
	if ( estVideFile(*f) ) return NULL;

	// File temporaire pour pas casser la FileAttente
	File tmp = allocFile();
	if ( !tmp ) return NULL;

	// le minimum par defaut est le 1er element de la file
	ProcessusIterator* min = (ProcessusIterator*) defilerFile(*f);
	while ( !estVideFile(*f) ) {
		ProcessusIterator* p = (ProcessusIterator*) defilerFile(*f);
	if (p->tempsRestant < min->tempsRestant ||
	   (p->tempsRestant == min->tempsRestant &&
	    p->processus->timeArrival < min->processus->timeArrival)) {
			enfilerFile(tmp, min);			// ancient min retourn dans la file temporaire
			min = p;
		} else {
			enfilerFile(tmp,p);				// p retourn dans la file temporaire
		}
	}

	// Remettre les autres dans la file d'origine (qui est maintenant vide)
    while (!estVideFile(tmp)) {
        enfilerFile(*f, defilerFile(tmp));
    }
	destroyFile(tmp,NULL);
	return min;
}


/**
 * 
 * */
static void traiterUC(ProcessusIterator** surLeCPU,File *fileAttente, File *fileES, ExecutionTimeline *resultat, int *nbProcessusTraiter){
	/**
	 * Vérifivation de la disponibilité du CPU
	 * et selectionne le processus avec le
	 * minimum de temps d'UC
	 * */
	if ( *surLeCPU == NULL ) {
		// WARNING : surLeCPU peut être NULL si fileAttente est vide
		*surLeCPU = estMinTempsUCProcessus(fileAttente);
	} 


	/** 
	 * On avancer le CPU d'un tick.
	 * De plus, on vérifie si le processus qui utilise le CPU a maintenant des temps ES
	 * sinon si le processus est fini alors dans les 2 cas personne n'utilise le CPU.
	 * 
	 * Ici, si on met pas surLeCPU à NULL quand le processus passe en ES ou est terminé
	 * alors au tick suivant on va appeler avancerIterator(surLeCPU) sur un 
	 * processus soit en ES soit terminé ce qui est faut 
	 * */
	if ( *surLeCPU != NULL ) {
		avancerIterator(*surLeCPU);
		Processus* p = getTimelineProcessus(resultat, *surLeCPU);
		pushOrMergeOperationProcessus(p->listeTQ, UC, 1);

		if ( iteratorEstFini(*surLeCPU) ) {
			(*nbProcessusTraiter)++;
			free(*surLeCPU);
			*surLeCPU = NULL;
		} else if ( etatIterator(*surLeCPU) == ES ) {
			//printf("  [traiterUC] %s passe en ES\n", (*surLeCPU)->processus->name);
			enfilerFile(*fileES, *surLeCPU);	
			*surLeCPU = NULL; 
		} 

	}
}


/**
 * 
 * */
static void traiterES(File *fileES, File *fileAttente, ExecutionTimeline *resultat, int *nbProcessusTraiter) {
	if ( !fileES || estVideFile(*fileES) ) return;


	File tmp = allocFile();
	if ( !tmp ) return;

	while ( !estVideFile(*fileES) ) {
		ProcessusIterator* it = (ProcessusIterator*) defilerFile(*fileES);
		avancerIterator(it);
		//printf("  [traiterES] it=%s etat=%d tempsRestant=%d\n", it->processus->name, etatIterator(it), it->tempsRestant);

		Processus *p = getTimelineProcessus(resultat, it);
		pushOrMergeOperationProcessus(p->listeTQ, ES, 1);
		// Si le temps ES est fini et qu'il a encore des temps de PCU
		if ( etatIterator(it) == UC ) {
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
 * 
 * */
static void traiterWait(File *fileAttente, ExecutionTimeline *resultat){
	if ( !fileAttente || estVideFile(*fileAttente) ) return;

	File tmp = allocFile();
	if ( !tmp ) return;

	while ( !estVideFile(*fileAttente) ) {
		ProcessusIterator* it = (ProcessusIterator*) defilerFile(*fileAttente);
		Processus *p = getTimelineProcessus(resultat, it);
		pushOrMergeOperationProcessus(p->listeTQ, W, 1);
		enfilerFile(tmp, it);
	}
	destroyFile(*fileAttente, NULL);
	*fileAttente = tmp;
}