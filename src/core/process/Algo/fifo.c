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
void executerTeteFile_FIFO(File file, const ExecutionTimeline *timeline) {
//void executerTeteFile(File file, const ExecutionTimeline *timeline,int *nb) {
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
 * @brief Pilote la boucle de simulation principale de l'algorithme FIFO.
 *
 * Cette fonction initialise l'environnement de simulation et itère tick par tick.
 * Elle opère en deux phases :
 * - Une phase de mise à jour où elle identifie les nouveaux arrivants et fait
 * progresser les processus en E/S.
 * - Une phase d'exécution où seule la tête de file est autorisée à consommer du CPU.
 * La simulation s'arrête Quand il n'y a plus de processus "vivant" dans le système.
 *
 * @param liste_tq La liste initiale des processus (et leurs cycles) à traiter.
 * @return Un pointeur vers la chronologie complète (Gantt) de l'exécution.
 */
ExecutionTimeline *fifo(ListeTQ liste_tq) {
//ExecutionTimeline *RRN(ListeTQ liste_tq, int nb) {

    ExecutionTimeline *timeline = allocTimeline();
    if (!timeline) return NULL;

    int n;
    ProcessusIterator *tab = createTabIteratorFromLTQ(liste_tq, &n);
    if (!tab) { destroyTimeline(timeline); return NULL; }

    initTimelineProcessus(timeline, liste_tq);

    int time = 0;
    File file = allocFile();
    initFile(file);

    //int restNb = nb;

    bool flag = true;
    while (flag) {

        if (countAlive(tab, n) == 0) flag = false;

        // Passe 1 : enregistrer les arrivants UC et avancer les ES
        for (int i = 0; i < n; i++) {

            if (iteratorEstFini(&tab[i])) continue;
            if (tab[i].processus->timeArrival > time) continue;
            if (teteFile(file) == &tab[i]) continue;

            Processus *pTimeline = getTimelineProcessus(timeline, &tab[i]);

            switch (etatIterator(&tab[i])) {
                case UC: traiterUC_FIFO_RRN(&tab[i], pTimeline, file); break;
                case ES: traiterES_FIFO_RRN(&tab[i], pTimeline); break;
                default: avancerIterator(&tab[i]); break;
            }
        }

        // Passe 2 : UN SEUL processus utilise l'UC ce tick (la tête de file)
        executerTeteFile_FIFO(file, timeline);


        time++;
    }

    destroyFile(file, NULL);
    free(tab);
    return timeline;
}