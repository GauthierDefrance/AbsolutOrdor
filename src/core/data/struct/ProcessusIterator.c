#include "ProcessusIterator.h"

void initIterator(Processus *p, ProcessusIterator *it) {
    if (!p || !it) return;

    it->processus = p;                 // On garde la référence vers le processus
    it->quantumCourant = p->listeTQ->tete;

    if (it->quantumCourant) {
        Quantum *q = it->quantumCourant->data;
        it->tempsRestant = q->nbQuantum;
    } else {
        it->tempsRestant = 0;          // Aucun quantum → déjà fini
    }
}

enum OperationProcessus etatIterator(ProcessusIterator *it) {
    if (!it || !it->quantumCourant)
        return -1; // FINI

    Quantum *q = it->quantumCourant->data;
    return q->type;
}

void avancerIterator(ProcessusIterator *it) {
    if (!it || !it->quantumCourant)
        return;

    it->tempsRestant--;

    // Quantum terminé → passer au suivant
    if (it->tempsRestant <= 0) {
        it->quantumCourant = it->quantumCourant->suivant;

        if (it->quantumCourant) {
            Quantum *q = it->quantumCourant->data;
            it->tempsRestant = q->nbQuantum;
        } else {
            it->tempsRestant = 0; // FINI
        }
    }
}

bool iteratorEstFini(ProcessusIterator *it) {
    return (!it || it->quantumCourant == NULL);
}
