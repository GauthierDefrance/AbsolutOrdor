/**
 * @file algo_commun.c
 * @brief Implémentation des outils de gestion des processus et de la chronologie.
 * Ce module contient les fonctions de manipulation de listes, de recherche
 * de candidat et de mise à jour des états CPU/ES/Wait.
 */


#include "algo_commun.h"


/**
 * @brief Trie une liste de processus par ordre chronologique d'arrivée.
 *
 * Utilise un algorithme de tri par sélection pour garantir que les processus
 * sont injectés dans le système selon leur `timeArrival`.
 *
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
 * @brief Recherche et extrait le processus avec le temps de calcul restant le plus court.
 *
 * Parcourt la file pour identifier le candidat optimal. En cas d'égalité sur le
 * temps restant, le processus arrivé le plus tôt en file est prioritaire.
 *
 * @param f Pointeur vers la file (Ready Queue) à analyser.
 * @return ProcessusIterator* Le processus le plus court (extrait de la file).
 */
ProcessusIterator* retirerMinTempsUC(const File* f) {
    if (*f == NULL || estVideFile(*f)) return NULL;

    File tmp = allocFile();
    ProcessusIterator* min = NULL;
    int n = tailleFile(*f);

    for (int i = 0; i < n; i++) {
        ProcessusIterator* p = defilerFile(*f);

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


/**
 * @brief Exécute une unité de temps sur le processeur.
 *
 * Décrémente le temps restant du bloc UC actuel et gère les transitions
 * vers l'état "Terminé" ou "E/S" (ES).
 *
 * @param surLeCPU Double pointeur vers le processus actif (mis à NULL si fini/bloqué).
 * @param fileES File où envoyer le processus s'il entame un cycle d'ES.
 * @param resultat Chronologie à mettre à jour pour l'enregistrement CPU.
 * @param nbProcessusTraiter Compteur de processus terminés à incrémenter.
 */
void traiterUC(ProcessusIterator** surLeCPU, const File *fileES, const ExecutionTimeline *resultat, int *nbProcessusTraiter) {
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
 * @brief Gère l'admission et la mise en attente des processus candidats au CPU.
 *
 * Pour chaque processus en phase de calcul, cette fonction vérifie s'il est déjà
 * présent dans la file d'attente. Si ce n'est pas le cas, il y est inséré.
 * Elle comptabilise également le temps d'attente (Wait) si le processus est bloqué
 * derrière un autre occupant déjà la tête de file.
 *
 * @param it L'itérateur représentant l'état actuel du processus.
 * @param pTimeline Référence du processus dans la chronologie pour l'enregistrement.
 * @param file La file d'attente (Ready Queue) où le processus doit patienter.
 */
void traiterUC_FIFO_RRN(ProcessusIterator *it, const Processus *pTimeline, File file) {
    //Si le processus était déjà en attente avant, on le laisse
    if (enAttenteIterator(it)) {
        pushOrMergeOperationProcessus(pTimeline->listeTQ, W, 1);
        return;
    }

    bool estVide = estVideFile(file); // ← vérifier AVANT d'enfiler
    enfilerFile(file, it);
    it->enAttente = true;

    if (!estVide) {
        // Quelqu'un devant → on attend ce tick
        pushOrMergeOperationProcessus(pTimeline->listeTQ, W, 1);
    }
    // Si file était vide : pas de W ici,
}


/**
 * @brief Simule la progression d'une opération d'Entrée/Sortie (E/S).
 * Fait progresser l'itérateur du processus pour ses cycles d'E/S. Contrairement
 * à l'UC, les E/S peuvent être traitées de manière concurrente pour tous les
 * processus concernés. Les résultats sont fusionnés dans la timeline pour
 * optimiser la lecture du diagramme final.
 *
 * @param it L'itérateur du processus actuellement bloqué en E/S.
 * @param pTimeline Référence du processus pour l'enregistrement de l'opération ES.
 */
void traiterES_FIFO_RRN(ProcessusIterator *it, const Processus *pTimeline) {
    Quantum *completed = avancerIterator(it);
    if (completed) {
        pushOrMergeOperationProcessus(pTimeline->listeTQ, ES, completed->nbQuantum);
    }
}