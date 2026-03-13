#include "ExecutionTimeline.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Alloue une ExecutionTimeline vide.
 * Elle contient simplement une ListeTQ de Processus*.
 */
ExecutionTimeline *allocTimeline() {
    ExecutionTimeline *tl = malloc(sizeof(ExecutionTimeline));
    if (!tl) return NULL;

    tl->processus = allocMemLTQ();
    initLTQ(tl->processus);

    return tl;
}

/**
 * Libère la timeline.
 * ATTENTION : ne libère PAS les Processus* eux-mêmes,
 * car ils appartiennent à l’ordonnanceur ou au module appelant.
 */
void destroyTimeline(ExecutionTimeline *tl) {
    if (!tl) return;

    destroyLTQ(tl->processus, NULL);
    free(tl);
}

/**
 * Ajoute un processus (déjà ordonnancé) dans la timeline.
 * L’ordonnanceur aura modifié sa listeTQ pour inclure UC/ES/W.
 */
void ajouterProcessusTimeline(ExecutionTimeline *tl, Processus *p) {
    if (!tl || !p) return;
    inserQueueLTQ(tl->processus, p);
}





/**
 * Initialise la timeline avec une copie (sans quantums) de chaque processus.
 * L'ordre d'insertion est l'ordre d'arrivée dans la ListeTQ.
 */
void initTimelineProcessus(ExecutionTimeline *timeline, ListeTQ liste_tq) {
    Liste elem = teteLTQ(liste_tq);
    while (elem != NULL) {
        Processus *src   = (Processus *)(elem->data);
        Processus *copie = deepCopyProcessusWithoutQuantums(src);
        inserQueueLTQ(timeline->processus, copie);
        elem = suivantListe(elem);
    }
}

/**
 * Retourne le Processus* dans la timeline correspondant à l'iterator donné.
 * On retrouve le bon processus par comparaison de nom.
 */
Processus *getTimelineProcessus(ExecutionTimeline *timeline, ProcessusIterator *it) {
    Liste elem = teteLTQ(timeline->processus);
    while (elem != NULL) {
        Processus *p = (Processus *)(elem->data);
        if (strcmp(p->name, it->processus->name) == 0)
            return p;
        elem = suivantListe(elem);
    }
    return NULL;
}