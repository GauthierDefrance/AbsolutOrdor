/**
 * @file ExecutionTimeline.c
 * @brief Implémentation du journal d'exécution des processus.
 */


#include "ExecutionTimeline.h"
#include <stdlib.h>
#include <stdio.h>


/**
 * @brief Alloue la mémoire pour la Timeline et son conteneur interne.
 *
 * Alloue une ExecutionTimeline vide.
 * Elle contient simplement une ListeTQ de Processus*.
 *
 * @return ExecutionTimeline* Pointeur vers la structure allouée.
 */
ExecutionTimeline *allocTimeline() {
    ExecutionTimeline *tl = malloc(sizeof(ExecutionTimeline));
    if (!tl) return NULL;

    tl->processus = allocMemLTQ();
    initLTQ(tl->processus);

    return tl;
}

/**
 * @brief Libération de la Timeline.
 *
 * @waring Cette fonction ne libère pas les données (Processus*)
 * pour tout libérer, il faut passer `libMemProcessus` à `destroyLTQ`.
 *
 * @param tl La timeline à détruire.
 */
void destroyTimeline(ExecutionTimeline *tl) {
    if (!tl) return;

    destroyLTQ(tl->processus, NULL);
    free(tl);
}


/**
 * @brief Enregistre un processus dans l'historique global.
 *
 * Ajoute un processus (déjà ordonnancé) dans la timeline.
 * L’ordonnanceur aura modifié sa listeTQ pour inclure UC/ES/W.
 *
 * @param tl La timeline.
 * @param p  Pointeur vers le processus dont l'exécution est terminée.
 */
void ajouterProcessusTimeline(ExecutionTimeline *tl, Processus *p) {
    if (!tl || !p) return;
    inserQueueLTQ(tl->processus, p);
}


/**
 * @brief Prépare la timeline pour une nouvelle simulation.
 *
 * Cette fonction est cruciale : elle parcourt la liste des processus
 * chargés (ex: depuis un CSV) et crée pour chacun une copie "vierge" dans
 * la timeline via `deepCopyProcessusWithoutQuantums`. Cela permet de
 * commencer à remplir leur historique de quanta (UC/ES/W) à partir de zéro.
 *
 * @param timeline La timeline à remplir.
 * @param liste_tq La liste source des processus.
 */
void initTimelineProcessus(const ExecutionTimeline *timeline, ListeTQ liste_tq) {
    Liste elem = teteLTQ(liste_tq);
    while (elem != NULL) {
        Processus *src   = (Processus *)(elem->data);
        Processus *copie = deepCopyProcessusWithoutQuantums(src);
        inserQueueLTQ(timeline->processus, copie);
        elem = suivantListe(elem);
    }
}


/**
 * @brief Retrouve un processus de la timeline correspondant à un itérateur.
 *
 * La comparaison s'effectue sur le nom du processus.
 * C'est ce qui permet de relier un processus "actif" dans l'ordonnanceur
 * à sa "fiche de score" dans la timeline.
 *
 * @param timeline La timeline où chercher.
 * @param it L'itérateur (contenant souvent un pointeur vers le processus d'origine).
 * @return Processus* Le pointeur vers le processus stocké dans la timeline.
 */
Processus *getTimelineProcessus(const ExecutionTimeline *timeline, const ProcessusIterator *it) {
    Liste elem = teteLTQ(timeline->processus);
    while (elem != NULL) {
        Processus *p = (Processus *)(elem->data);
        if (strcmp(p->name, it->processus->name) == 0)
            return p;
        elem = suivantListe(elem);
    }
    return NULL;
}