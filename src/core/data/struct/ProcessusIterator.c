#include "ProcessusIterator.h"


void initIterator(Processus *p, ProcessusIterator *it) {
    if (!p || !it) return;

    it->processus = p;
    it->enAttente = false;
    it->tempsEntreeFile = 0;
    it->quantumCourant = p->listeTQ->tete;

    if (it->quantumCourant) {
        Quantum *q = it->quantumCourant->data;
        it->tempsRestant = q->nbQuantum;
    } else {
        it->tempsRestant = 0;
    }
}

enum OperationProcessus etatIterator(ProcessusIterator *it) {
    if (!it || !it->quantumCourant)
        return -1; // FINI

    Quantum *q = it->quantumCourant->data;
    return q->type;
}

Quantum *avancerIterator(ProcessusIterator *it) {
    if (!it || !it->quantumCourant) return NULL;

    it->tempsRestant--;

    if (it->tempsRestant <= 0) {
        Quantum *completed = (Quantum *)it->quantumCourant->data;

        it->quantumCourant = it->quantumCourant->suivant;
        if (it->quantumCourant) {
            Quantum *q = (Quantum *)it->quantumCourant->data;
            it->tempsRestant = q->nbQuantum;
        }

        return completed;
    }

    return NULL;
}

bool iteratorEstFini(ProcessusIterator *it) {
    return (!it || it->quantumCourant == NULL);
}

/**
 * Crée un tableau de ProcessusIterator depuis une ListeTQ de Processus*.
 *
 * - Parcourt la ListeTQ pour compter les processus
 * - Alloue un tableau de N ProcessusIterator
 * - Initialise chaque iterator avec le Processus* correspondant
 *
 * @param listeTQ  ListeTQ de Processus*
 * @param outSize  [OUT] nombre d'éléments dans le tableau retourné
 * @return         tableau alloué (à libérer avec free()), ou NULL si erreur
 */
ProcessusIterator *createTabIteratorFromLTQ(ListeTQ listeTQ, int *outSize) {
    if (!listeTQ || !outSize) return NULL;

    // 1. Compter les processus
    int n = 0;
    for (Liste c = listeTQ->tete; c != NULL; c = c->suivant) n++;

    *outSize = n;

    if (n == 0) return NULL;

    // 2. Allouer le tableau
    ProcessusIterator *tab = malloc(n * sizeof(ProcessusIterator));
    if (!tab) {
        fprintf(stderr, "Erreur : echec allocation tableau d'iterateurs\n");
        return NULL;
    }

    // 3. Initialiser chaque iterator
    int i = 0;
    for (Liste c = listeTQ->tete; c != NULL; c = c->suivant) {
        Processus *p = (Processus *)c->data;
        initIterator(p, &tab[i]);
        i++;
    }

    return tab;
}


bool enAttenteIterator(const ProcessusIterator *it) {
    if (it==NULL) return false;
    return it->enAttente;
}


/**
 * Compte le nombre d'iterators encore en vie
 */
int countAlive(ProcessusIterator *tab, int n) {
    int alive = 0;
    for (int i = 0; i < n; i++) {
        if (!iteratorEstFini(&tab[i]))
            alive++;
    }
    return alive;
}