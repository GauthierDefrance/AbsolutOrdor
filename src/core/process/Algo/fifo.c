/**
 * @file fifo.c
 * @brief Implémentation de l'ordonnanceur First-In, First-Out (FIFO).
 * * Ce module simule un ordonnancement de type "premier arrivé, premier servi".
 * La gestion des processus repose sur une file (Ready Queue) où le processus
 * en tête dispose de l'usage exclusif du processeur jusqu'à ce qu'il se bloque
 * (E/S) ou qu'il se termine. Aucun mécanisme de préemption n'est appliqué ici.
 */


#include "fifo.h"


/**
 * @brief Orchestre l'exécution du processus détenteur du CPU.
 *
 * Cette fonction identifie le processus en tête de la file d'attente et lui
 * alloue une unité de temps (tick). Elle assure la mise à jour de la chronologie
 * et gère la sortie du processus de la Ready Queue si celui-ci entame une phase
 * d'E/S ou s'achève, permettant ainsi au processus suivant de devenir la nouvelle tête.
 *
 * @param file La file d'attente contenant les processus prêts (Ready Queue).
 * @param timeline Structure de résultats où l'activité UC est enregistrée.
 */
void executerTeteFile(File file, const ExecutionTimeline *timeline) {
    //Si la tete de file est NULL on ne fait rien
    if (estVideFile(file)) return;

    //On obtient le Processus associé à notre iterator
    ProcessusIterator *it = (ProcessusIterator *)teteFile(file);
    Processus *pTimeline = getTimelineProcessus(timeline, it);

    //On éxécute le processus en tête
    avancerIterator(it);
    pushOrMergeOperationProcessus(pTimeline->listeTQ, UC, 1);


    // Si le processus passe à ES ou est fini, on defile.
    // On laisse alors un autre processus potentiellement
    if (etatIterator(it) == ES || iteratorEstFini(it)) {
        it->enAttente = false;
        defilerFile(file);
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
void traiterUC_FIFO(ProcessusIterator *it, const Processus *pTimeline, File file) {

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
void traiterES_FIFO(ProcessusIterator *it, const Processus *pTimeline) {
    Quantum *completed = avancerIterator(it);
    if (completed) {
        pushOrMergeOperationProcessus(pTimeline->listeTQ, ES, completed->nbQuantum);
    }
}


/**
 * @brief Pilote la boucle de simulation principale de l'algorithme FIFO.
 *
 * Cette fonction initialise l'environnement de simulation et itère tick par tick.
 * Elle opère en deux phases :
 * - Une phase de mise à jour où elle identifie les nouveaux arrivants et fait
 * progresser les processus en E/S.
 * - Une phase d'exécution où seule la tête de file est autorisée à consommer du CPU.
 * La simulation s'arrête lorsqu'aucun processus n'est plus "vivant" dans le système.
 *
 * @param liste_tq La liste initiale des processus (et leurs cycles) à traiter.
 * @return Un pointeur vers la chronologie complète (Gantt) de l'exécution.
 */
ExecutionTimeline *fifo(ListeTQ liste_tq) {

    ExecutionTimeline *timeline = allocTimeline();
    if (!timeline) return NULL;

    int n;
    ProcessusIterator *tab = createTabIteratorFromLTQ(liste_tq, &n);
    if (!tab) { destroyTimeline(timeline); return NULL; }

    initTimelineProcessus(timeline, liste_tq);

    int time = 0;
    File file = allocFile();
    initFile(file);

    while (true) {

        if (countAlive(tab, n) == 0) break;

        // Passe 1 : enregistrer les arrivants UC et avancer les ES
        for (int i = 0; i < n; i++) {

            if (iteratorEstFini(&tab[i])) continue;
            if (tab[i].processus->timeArrival > time) continue;
            if (teteFile(file) == &tab[i]) continue;

            Processus *pTimeline = getTimelineProcessus(timeline, &tab[i]);

            switch (etatIterator(&tab[i])) {
                case UC: traiterUC_FIFO(&tab[i], pTimeline, file); break;
                case ES: traiterES_FIFO(&tab[i], pTimeline); break;
                default: avancerIterator(&tab[i]); break;
            }
        }

        // Passe 2 : UN SEUL processus utilise l'UC ce tick (la tête de file)
        executerTeteFile(file, timeline);

        time++;
    }

    destroyFile(file, NULL);
    free(tab);
    return timeline;
}