/**
 * @file ProcessusIterator.c
 * @brief Implémentation des mécanismes d'avancement du temps de simulation.
 */


#include "ProcessusIterator.h"


/**
 * @brief Prépare l'itérateur pour le début de la simulation.
 *
 * Extrait le premier quantum de la liste du processus et initialise le décompte.
 * Si le processus n'a aucun quantum défini, il est immédiatement considéré comme fini.
 *
 * @param p Processus source.
 * @param it Itérateur cible.
 */
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


/**
 * @brief Analyse le type de quantum pointé par l'itérateur.
 *
 * Cette fonction permet à l'ordonnanceur de savoir si le processus demande
 * le CPU (UC) ou une ressource d'E/S (ES).
 *
 * @param it L'itérateur actif.
 * @return Le type d'opération ou -1 si `quantumCourant` est NULL.
 */
enum OperationProcessus etatIterator(ProcessusIterator *it) {
    if (!it || !it->quantumCourant)
        return -1; // FINI

    Quantum *q = it->quantumCourant->data;
    return q->type;
}


/**
 * @brief Gère la transition entre les phases d'un processus.
 *
 * C'est le coeur du moteur : à chaque appel, le temps restant diminue.
 * Si le compteur tombe à zéro, la fonction effectue une transition vers le
 * noeud suivant de la liste chaînée.
 * @param it L'itérateur à avancer.
 * @return Un pointeur vers le Quantum qui vient d'être achevé (utile pour les stats).
 */
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


/**
 * @brief Détermine la fin de vie d'un processus dans la simulation.
 *
 * @param it Itérateur à tester.
 * @return true si le curseur a dépassé le dernier quantum de la liste.
 */
bool iteratorEstFini(ProcessusIterator *it) {
    return (!it || it->quantumCourant == NULL);
}


/**
 * @brief Alloue et initialise un ensemble d'itérateurs.
 *
 * Effectue une première passe pour compter les éléments, alloue le bloc mémoire
 * contigu nécessaire, puis effectue une seconde passe pour initialiser chaque
 * case du tableau avec les données du processus correspondant.
 *
 * @param listeTQ Liste des processus sources.
 * @param outSize Pointeur de sortie pour la taille du tableau.
 * @return Un tableau d'itérateurs alloué sur le tas.
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


/**
 * @brief Accesseur simple pour l'état d'attente.
 */
bool enAttenteIterator(const ProcessusIterator *it) {
    if (it==NULL) return false;
    return it->enAttente;
}


/**
 * @brief Calcule le nombre de processus encore présents dans le système.
 *
 * @param tab Tableau d'itérateurs.
 * @param n Taille du tableau.
 * @return Nombre de processus dont l'itérateur n'est pas "Fini".
 */
int countAlive(ProcessusIterator *tab, int n) {
    int alive = 0;
    for (int i = 0; i < n; i++) {
        if (!iteratorEstFini(&tab[i]))
            alive++;
    }
    return alive;
}