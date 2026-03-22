#include "TimelineStatsCalculator.h"
#include <limits.h>


int nbProcesssuOperation(Processus *processus, enum OperationProcessus operationProcessus) {
    Liste liste = processus->listeTQ->tete;
    int n = 0;
    while (!estVideListe(liste)) {
        Quantum *q = (Quantum *) liste->data;
        if (q->type == operationProcessus) n++;
        liste = suivantListe(liste);
    }
    return n;
}

double attenteMoyProcessus(ExecutionTimeline *timeline) {
    int nbProcessus = 0, attenteTotal = 0;
    Liste liste = timeline->processus->tete;
    while (!estVideListe(liste)) {
        attenteTotal += nbProcesssuOperation((Processus *) liste->data, W);
        nbProcessus++;
        liste = suivantListe(liste);
    }
    return (double) attenteTotal / (double) nbProcessus;
}

int tempsRestitutionProcessus(Processus *processus) {
    Liste liste = processus->listeTQ->tete;
    int n = -processus->timeArrival;
    while (!estVideListe(liste)) {
        Quantum *q = (Quantum *) liste->data;
        n += q->nbQuantum;
        liste = suivantListe(liste);
    }
    return n;
}

double restitutionMoyProcessus(ExecutionTimeline *timeline) {
    int nbProcessus = 0, restitutionTotal = 0;
    Liste liste = timeline->processus->tete;
    while (!estVideListe(liste)) {
        restitutionTotal += tempsRestitutionProcessus((Processus *) liste->data);
        nbProcessus++;
        liste = suivantListe(liste);
    }
    return (double) restitutionTotal / (double) nbProcessus;
}

int tempsRepProcessus(Processus *processus) {
    Liste liste = processus->listeTQ->tete;
    int n = 0;
    bool flag = true;
    while (!(estVideListe(liste) && flag)) {
        Quantum *q = (Quantum *) liste->data;
        if (q->type != W) flag = false;
        n++;
        liste = suivantListe(liste);
    }
    return n;
}

double tempRepMoyProcessus(ExecutionTimeline *timeline) {
    int nbProcessus = 0, tempRepTotal = 0;
    Liste liste = timeline->processus->tete;
    while (!estVideListe(liste)) {
        tempRepTotal += tempsRepProcessus((Processus *) liste->data);
        nbProcessus++;
        liste = suivantListe(liste);
    }
    return (double) tempRepTotal / (double) nbProcessus;
}

double tauxOccupationCPU(ExecutionTimeline *timeline) {
    if (!timeline || !timeline->processus) return 0.0;

    int tMax = getTimelineMax(timeline);
    if (tMax == 0) return 0.0;

    bool *cpuActif = calloc(tMax + 1, sizeof(bool));
    if (!cpuActif) return 0.0;

    for (Liste cell = timeline->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus *) cell->data;
        int t = p->timeArrival;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            Quantum *q = (Quantum *) qc->data;
            for (int i = 0; i < q->nbQuantum; i++) {
                if (q->type == UC) cpuActif[t] = true;
                t++;
            }
        }
    }

    int ticksCPUActif = 0;
    for (int t = 0; t < tMax; t++)
        if (cpuActif[t]) ticksCPUActif++;

    free(cpuActif);
    return ((double) ticksCPUActif / (double) tMax) * 100.0;
}

int getTimelineMax(const ExecutionTimeline *timeline) {
    if (!timeline || !timeline->processus) return 0;
    int tMax = 0;
    for (Liste cell = timeline->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus *) cell->data;
        int t = p->timeArrival;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant)
            t += ((Quantum *) qc->data)->nbQuantum;
        if (t > tMax) tMax = t;
    }
    return tMax;
}

int getTimelineDebut(const ExecutionTimeline *timeline) {
    if (!timeline || !timeline->processus) return 0;
    int tDebut = INT_MAX;
    for (Liste cell = timeline->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus *) cell->data;
        if (p->timeArrival < tDebut) tDebut = p->timeArrival;
    }
    return (tDebut == INT_MAX) ? 0 : tDebut;
}

int getTimelineNbProcessus(const ExecutionTimeline *timeline) {
    if (!timeline || !timeline->processus) return 0;
    int n = 0;
    for (Liste cell = timeline->processus->tete; cell; cell = cell->suivant)
        n++;
    return n;
}

int getTimelineTicksType(const ExecutionTimeline *timeline, enum OperationProcessus type) {
    if (!timeline || !timeline->processus) return 0;
    int total = 0;
    for (Liste cell = timeline->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus *) cell->data;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            Quantum *q = (Quantum *) qc->data;
            if (q->type == type) total += q->nbQuantum;
        }
    }
    return total;
}