#include "fifo.h"
#include "data/struct/ProcessusIterator.h"

/* ==============================================
    Traitement par type d'opération
   ============================================== */

/**
 * Fonction qui est appellé en boucle avant tout le monde dans FIFO,
 * elle donne la prio
 * @param file
 * @param timeline
 */
static void executerTeteFile(File file, ExecutionTimeline *timeline) {
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
 *
 * @param it
 * @param pTimeline
 * @param file
 */
static void traiterUC(ProcessusIterator *it, Processus *pTimeline, File file) {

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
    // Si file était vide → pas de W ici,
    // executerTeteFile() s'occupe de l'UC dans la passe 2
}

/**
 *
 * @param it
 * @param pTimeline
 */
void traiterES(ProcessusIterator *it, Processus *pTimeline) {
    Quantum *completed = avancerIterator(it);
    if (completed) {
        pushOrMergeOperationProcessus(pTimeline->listeTQ, ES, completed->nbQuantum);
    }
}

/* ==============================================
    Boucle principale FIFO
   ============================================== */


/**
 *
 * @param liste_tq
 * @return
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
                case UC: traiterUC(&tab[i], pTimeline, file); break;
                case ES: traiterES(&tab[i], pTimeline); break;
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