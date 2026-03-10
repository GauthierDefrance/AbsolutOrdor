#include "sjf.h"



/**
 * @brief Réalise l'algorithme SJF (Shortest Job First) d'ordonnancement
 *        de processus non préemptif.
 * @note  Sélectionne à chaque étape le processus disponible avec le plus
 *        petit nombre de quantums. Si aucun processus n'est disponible
 *        au temps courant, avance au prochain temps d'arrivée.
 * @param tab - Tableau des processus à ordonnancer.
 * @param taille - Nombre de processus dans le tableau.
 * @return Resultat* - Pointeur vers le résultat de l'ordonnancement,
 *                     NULL en cas d'échec d'allocation mémoire.
 */
Resultat* sjf(Processus tab[], int taille) {
	//printf("dans sjf\n");

	//int n = nbQuantumTotal(tab, taille);
	int temps_courant = 0;					// Temps courant
	int nbProcessusTraiter = 0;
	

	Resultat *resultat = allocMemResultat(taille);
	if ( resultat == NULL ) return NULL ; 
	initResultat(resultat);
	

	// Tableau indicé par le numero du processus
	bool processusTraiter[taille];
	for ( int h = 0; h < taille; h++ ){ processusTraiter[h] = false; }	
    //printf("apres init tableau\n");

	// Tant que tout les processus sont pas traiter
	while ( nbProcessusTraiter < taille ) {
        //printf("tour while : %d\n", nbProcessusTraiter);
		int minQuantum = estMinQuantum(tab, processusTraiter, taille, temps_courant);	// Quantum minimum parmis ceux non traiter
	    //printf("minQuantum : %d\n", minQuantum);
		int bestCandidat = -1;
		

		// On selectionne le meilleur candidat
		for ( int i = 0; i < taille; i++) {
			if ( ( processusTraiter[i] == false ) && ( getTimeArrival(tab[i]) <= temps_courant ) && ( getNbQuantum(tab[i]) == minQuantum ) ) {
				bestCandidat = i; // On mémorise l'indice du meilleur candidat
			}
		}
    	//printf("bestCandidat : %d\n", bestCandidat);

		/**
		 * Aucun candidat pour ce temps
		 * on avance jusqu'au plus petit temps d'arrivé
		 * d'un processus non traiter
		 * pour par perdre du temps
		 * */
		if ( bestCandidat == -1 ) {
			temps_courant = MinTimeArrival(tab, processusTraiter, taille);
	        //printf("nouveau temps_courant : %d\n", temps_courant);
			continue;	// Permet de refaire la boucle while avec le nouveau temps courant
		}
		

		// traitement du meilleur candidat
		//printf("traitement de : %s\n", tab[bestCandidat].name);
		resultat->tabProcessus[nbProcessusTraiter] = tab[bestCandidat];
		//printf("apres tabProcessus\n");
		for ( int j = 0; j < getNbQuantum(tab[bestCandidat]); j++){
		    //printf("insertion tick %d\n", j);
		    inserQueueLTQ(resultat->listeTQ, nbProcessusTraiter);
		}
		//printf("apres insertion LTQ\n");
		

		// Mise à jour
		temps_courant += getNbQuantum(tab[bestCandidat]);
		processusTraiter[bestCandidat] = true;
		nbProcessusTraiter ++;
	}
	return resultat;
}


/**
 * @brief Calcule le nombre total de quantums de tous les processus.
 * @note  Utilisé pour déterminer la taille de la listeTQ du résultat.
 * @param tab - Tableau des processus.
 * @param taille - Nombre de processus dans le tableau.
 * @return int - Somme des nbQuantum de tous les processus.
 */
int nbQuantumTotal(Processus tab[], int taille) {
	int quantumTotal = 0;
	for ( int i = 0; i < taille; i++) {
		quantumTotal += getNbQuantum(tab[i]);
	}
	return quantumTotal;
}


/**
 * @brief Retourne le nombre de quantums minimum parmi les processus
 *        non encore traités.
 * @note  Utilisé par sjf() pour identifier le meilleur candidat.
 * @param tab - Tableau des processus.
 * @param processusTraiter - Tableau de booléens indiquant les processus
 *                           déjà traités (true = traité).
 * @param taille - Nombre de processus dans le tableau.
 * @return int - Valeur minimale de nbQuantum parmi les non traités.
 */
int estMinQuantum(Processus tab[], bool processusTraiter[], int taille, int temps_courant){
    int min = -1;
    for ( int i = 0; i < taille; i++){
        if ( processusTraiter[i] == false && getTimeArrival(tab[i]) <= temps_courant ) {
            if ( min == -1 || getNbQuantum(tab[i]) < min ) {
                min = getNbQuantum(tab[i]);
            }
        }
    }
    return min;
}


/**
 * @brief Retourne le temps d'arrivée minimum parmi les processus
 *        non encore traités.
 * @note  Utilisé par sjf() pour avancer le temps courant quand aucun
 *        processus n'est disponible.
 * @param tab - Tableau des processus.
 * @param processusTraiter - Tableau de booléens indiquant les processus
 *                           déjà traités (true = traité).
 * @param taille - Nombre de processus dans le tableau.
 * @return int - Valeur minimale de timeArrival parmi les non traités.
 */
int MinTimeArrival(Processus tab[], bool processusTraiter[], int taille){
    int min = -1;
    for ( int i = 0; i < taille; i++){
        //printf("tab[%d] timeArrival=%d nbQuantum=%d traite=%d\n", i, tab[i].timeArrival, tab[i].nbQuantum, processusTraiter[i]);
        if ( processusTraiter[i] == false ) {
            if ( min == -1 || getTimeArrival(tab[i]) < min ) {
                min = getTimeArrival(tab[i]);
            }
        }
    }
    //printf("MinTimeArrival retourne : %d\n", min);
    return min;
}