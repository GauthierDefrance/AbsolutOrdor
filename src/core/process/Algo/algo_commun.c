#include "algo_commun.h"
/**
 * @brief Trie une liste de processus par ordre chronologique d'arrivée.
 * Utilise un algorithme de tri par sélection pour garantir que les processus
 * sont injectés dans le système selon leur `timeArrival`.
 * @param ltq ListeTQ source non triée.
 * @return ListeTQ Nouvelle liste triée (copie des pointeurs).
 */
ListeTQ trieListe(ListeTQ ltq) {
    if (!ltq || teteLTQ(ltq) == NULL) return NULL;

    ListeTQ resultat = allocMemLTQ();
    ListeTQ copieTravail = allocMemLTQ();

    // On recopie les pointeurs des processus dans une liste temporaire
    Liste courant = teteLTQ(ltq);
    while (courant != NULL) {
        inserQueueLTQ(copieTravail, courant->data);
        courant = suivantListe(courant);
    }

    // On extrait itérativement le processus avec le temps d'arrivée min
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

    // Libération de la liste temporaire (les cellules seulement)
    destroyLTQ(copieTravail, NULL);
    return resultat;
}


/**
 * @brief Extrait le processus ayant le plus court cycle UC restant de la file.
 * Implémente le coeur de la logique SJF. Parcourt la file pour identifier
 * le burst CPU le plus faible. En cas d'égalité, applique la règle FCFS (First Come First Served).
 * @param f Pointeur vers la file d'attente (Ready Queue).
 * @return ProcessusIterator* L'itérateur du processus sélectionné, retiré de la file.
 */
ProcessusIterator* retirerMinTempsUC(const File* f) {
	if (f == NULL || estVideFile(*f)) return NULL;

	File tmp = allocFile();
	ProcessusIterator* min = NULL;
	int n = tailleFile(*f); // On utilise notre fonction

	// On défile tout pour trouver le min
	for(int i = 0; i < n; i++) {
		ProcessusIterator* p = (ProcessusIterator*) defilerFile(*f);

		if (min == NULL || p->tempsRestant < min->tempsRestant ||
		   (p->tempsRestant == min->tempsRestant && p->processus->timeArrival < min->processus->timeArrival)) {

			// Si on avait déjà un min, on le remet dans la file temporaire
			if (min != NULL) enfilerFile(tmp, min);
			min = p;
		   } else {
		   	enfilerFile(tmp, p);
		   }
	}

	// On remet le reste dans la file originale
	while (!estVideFile(tmp)) {
		enfilerFile(*f, defilerFile(tmp));
	}
	destroyFile(tmp, NULL);

	return min; // On retourne le min, il a bien été retiré de la file 'f'
}