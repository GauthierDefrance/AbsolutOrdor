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
 * Affiche la timeline finale :
 * Pour chaque processus, on affiche sa liste de quantum complète.
 */
void afficherTimeline(ExecutionTimeline *tl) {
    if (!tl) return;

    printf("\n===== EXECUTION TIMELINE =====\n");

    Liste cell = tl->processus->tete;

    while (cell) {
        Processus *p = cell->data;
        printf("%s : ", p->name);

        Liste qcell = p->listeTQ->tete;
        while (qcell) {
            Quantum *q = qcell->data;

            switch (q->type) {
                case UC: printf("UC(%d)", q->nbQuantum); break;
                case ES: printf("ES(%d)", q->nbQuantum); break;
                case W:  printf("W(%d)",  q->nbQuantum); break;
            }

            if (qcell->suivant)
                printf(" -> ");

            qcell = qcell->suivant;
        }

        printf("\n");
        cell = cell->suivant;
    }

    printf("================================\n");
}
