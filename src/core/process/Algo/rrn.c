/**
 * @file rrn.c
 * @brief ImplÃ©mentation de l'ordonnanceur Round-robin (RR).
 * * Ce module simule un ordonnancement de type "tourniquet".
 * La gestion des processus repose sur une file (Ready Queue) oÃ¹ le processus
 * en tÃªte dispose de l'usage exclusif du processeur jusqu'Ã  ce qu'il se bloque
 * (E/S) ou qu'il se termine ou Qu'il utilise ses N Quantum.
 * Aucun mÃ©canisme de prÃ©emption n'est appliquÃ© ici.
 */

#include "rrn.h"


/**
 * @brief Orchestre l'exÃ©cution du processus dÃ©tenteur du CPU.
 *
 * Cette fonction identifie le processus en tÃªte de la file d'attente et lui
 * alloue une unitÃ© de temps (tick). Elle assure la mise Ã  jour de la chronologie
 * et gÃ¨re la sortie du processus de la Ready Queue si celui-ci entame une phase
 * d'E/S ou s'achÃ¨ve ou Ã  utiliser ces N Quantums,
 * permettant ainsi au processus suivant de devenir la nouvelle tÃªte.
 *
 * @param file La file d'attente contenant les processus prÃªts (Ready Queue).
 * @param timeline Structure de rÃ©sultats oÃ¹ l'activitÃ© UC est enregistrÃ©e.
 * @param nb Nombre de Quantums restant autorisÃ©s Avant de laisser sa place.
 */
void executerTeteFile_RRN(File file, const ExecutionTimeline *timeline,int *nb) {
    //Si la tete de file est NULL on ne fait rien
    if (estVideFile(file)) return;

    //On obtient le Processus associÃ© Ã  notre iterator
    ProcessusIterator *it = (ProcessusIterator *)teteFile(file);
    Processus *pTimeline = getTimelineProcessus(timeline, it);

    //On Ã©xÃ©cute le processus en tÃªte
    avancerIterator(it);
    pushOrMergeOperationProcessus(pTimeline->listeTQ, UC, 1);

    *nb = *nb-1;

    // Si le processus passe Ã  ES ou est fini, on defile.
    // On laisse alors un autre processus potentiellement
  if (etatIterator(it) == ES || iteratorEstFini(it) || *nb == 0 ) {
        it->enAttente = false;
        defilerFile(file);
        *nb = 0;
    }
}

bool enfilerFilePrio(File f, ProcessusIterator *processus,bool haveNotConsumQuantum) {
    if (!f || !f->ltq) return true;

    if (f->ltq->tete == NULL) {
        inserQueueLTQ(f->ltq, processus);
        return true;
    }

    Liste *pp;

    if (haveNotConsumQuantum) {
        pp = &f->ltq->tete;
        ProcessusIterator *it = (*pp)->data;
        if (!(*pp && it->quantumCourant == it->processus->listeTQ->tete &&it->tempsRestant ==((Quantum*)it->processus->listeTQ->tete->data)->nbQuantum)) {
            inserTeteLTQ(f->ltq, processus);
            return false;
        }
    }
    pp = &f->ltq->tete->suivant;

    bool flag = true;
    while (flag && *pp) {
        ProcessusIterator *it = (*pp)->data;
        if (!(it->quantumCourant == it->processus->listeTQ->tete &&it->tempsRestant ==((Quantum*)it->processus->listeTQ->tete->data)->nbQuantum))flag = false;
        else pp = &(*pp)->suivant;
    }

    if (*pp == NULL) {
        inserQueueLTQ(f->ltq, processus);
    } else {
        inserTete(pp, processus);
    }
    return true;
}

void traiterUC_prio(ProcessusIterator *it, const Processus *pTimeline, File file,bool haveNotConsumQuantum ) {
    bool estVide = estVideFile(file);
    it->enAttente = enfilerFilePrio(file, it,haveNotConsumQuantum);

    if (!estVide && it->enAttente) {
        // Quelqu'un devant â†’ on attend ce tick
        pushOrMergeOperationProcessus(pTimeline->listeTQ, W, 1);
    }
}


/**
 * @brief Pilote la boucle de simulation principale de l'algorithme RRN.
 *
 * Cette fonction initialise l'environnement de simulation et itÃ¨re tick par tick.
 * Elle opÃ¨re en deux phases :
 * - Une phase de mise Ã  jour oÃ¹ elle identifie les nouveaux arrivants et fait
 * progresser les processus en E/S.
 * - Une phase d'exÃ©cution oÃ¹ la tÃªte de file est autorisÃ©e Ã  consommer N Quantum du CPU puis laisse sa place.
 * La simulation s'arrÃªte Quand il n'y a plus de processus "vivant" dans le systÃ¨me.
 *
 * @param liste_tq La liste initiale des processus (et leurs cycles) Ã  traiter.
 * @param nb Nombre de Quantums autorisÃ©s Avant de laisser sa place.
 * @return Un pointeur vers la chronologie complÃ¨te (Gantt) de l'exÃ©cution.
 */
ExecutionTimeline *rrn(ListeTQ liste_tq, const int nb) {

    ExecutionTimeline *timeline = allocTimeline();
    if (!timeline) return NULL;

    int n;
    ListeTQ listeTriee = trieListe(liste_tq);
    ProcessusIterator *tab = createTabIteratorFromLTQ(listeTriee, &n);
    if (!tab) { destroyTimeline(timeline); return NULL; }

    initTimelineProcessus(timeline, listeTriee);

    int time = 0;
    File file = allocFile();
    initFile(file);

    int restNb = nb;

    bool flag = true;
    while (flag) {

        if (countAlive(tab, n) == 0) flag = false;

        // Passe 1 : enregistrer les arrivants UC et avancer les ES
        for (int i = n-1; i >= 0; i--) {

            if (iteratorEstFini(&tab[i])) continue;
            if (tab[i].processus->timeArrival > time) continue;
            if (teteFile(file) == &tab[i]) continue;

            Processus *pTimeline = getTimelineProcessus(timeline, &tab[i]);

            switch (etatIterator(&tab[i])) {
                case UC:
                    if (tab[i].processus->timeArrival == time) traiterUC_prio(&tab[i], pTimeline, file,restNb == nb);
                    else traiterUC_FIFO_RRN(&tab[i], pTimeline, file);
                    break;
                case ES: traiterES_FIFO_RRN(&tab[i], pTimeline); break;
                default: avancerIterator(&tab[i]); break;
            }
        }

        // Passe 2 : UN SEUL processus utilise l'UC ce tick (la tÃªte de file)

        executerTeteFile_RRN(file, timeline, &restNb);
         if (restNb == 0) {
             restNb = nb;
         }

        time++;
    }

    destroyLTQ(listeTriee, NULL);
    destroyFile(file, NULL);
    free(tab);
    return timeline;
}