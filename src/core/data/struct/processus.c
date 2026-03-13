#include "processus.h"

Processus* allocMemProcessus() {
    Processus *p = malloc(sizeof(Processus));
    if (!p) {
        fprintf(stderr, "Erreur : echec allocation Processus\n");
        return NULL;
    }
    return p;
}

void libMemProcessus(Processus *p) {
    if (p == NULL) return;
    destroyLTQ(p->listeTQ, free);
    free(p);
}

void initProcessus(Processus *processus) {
    if (processus==NULL) return;

    strncpy(processus->name, "", NBMAXCHAR - 1);
    processus->timeArrival = 0;

    processus->listeTQ = allocMemLTQ();
    initLTQ(processus->listeTQ);
}

const char *processusName(Processus *p) {
    if (p==NULL) return NULL;
    return p->name;
}

int getTimeArrival(Processus *p) {
    if (p==NULL) return -1;
    return p->timeArrival;
}

ListeTQ getListeTQProcessus(Processus *processus) {
    if (!processus) return NULL;
    return processus->listeTQ;
}


/**
 * Crée une copie profonde d'un Processus.
 * - Le nom et la date d'arrivée sont copiés par valeur.
 * - Chaque Quantum de la listeTQ est alloué et copié indépendamment.
 * Le Processus retourné est indépendant de l'original : libérer l'un
 * n'affecte pas l'autre. À libérer avec libMemProcessus().
 */
Processus *deepCopyProcessus(Processus *src) {
    if (!src) return NULL;

    Processus *copy = allocMemProcessus();
    if (!copy) return NULL;
    initProcessus(copy);

    strncpy(copy->name, src->name, NBMAXCHAR - 1);
    copy->name[NBMAXCHAR - 1] = '\0';
    copy->timeArrival = src->timeArrival;

    for (Liste c = src->listeTQ->tete; c != NULL; c = c->suivant) {
        Quantum *qSrc = (Quantum *)c->data;

        Quantum *qCopy = malloc(sizeof(Quantum));
        if (!qCopy) {
            libMemProcessus(copy);
            return NULL;
        }

        qCopy->type      = qSrc->type;
        qCopy->nbQuantum = qSrc->nbQuantum;

        inserQueueLTQ(copy->listeTQ, qCopy);
    }

    return copy;
}



Processus *deepCopyProcessusWithoutQuantums(Processus *src) {
    if (!src) return NULL;

    Processus *copy = allocMemProcessus();
    if (!copy) return NULL;
    initProcessus(copy);

    strncpy(copy->name, src->name, NBMAXCHAR - 1);
    copy->name[NBMAXCHAR - 1] = '\0';
    copy->timeArrival = src->timeArrival;

    return copy;
}




void afficherProcessus(Processus *p) {
    if (!p) {
        printf("Processus NULL\n");
        return;
    }

    printf("[PROCESSUS] Nom : %s | Arrivee : %d\n",
           p->name,
           p->timeArrival);

    printf("  Quantum : ");

    Liste courant = p->listeTQ->tete;
    while (courant) {
        Quantum *q = courant->data;
        printf("[%s:%d] ",
               q->type == UC ? "UC" :
               q->type == ES ? "ES" : "W",
               q->nbQuantum);

        courant = courant->suivant;
    }

    printf("\n");
}


Quantum *allocQuantum(enum OperationProcessus type, int nbQuantum) {
    Quantum *q = malloc(sizeof(Quantum));
    if (!q) {
        fprintf(stderr, "Erreur : echec allocation Quantum\n");
        return NULL;
    }
    q->type      = type;
    q->nbQuantum = nbQuantum;
    return q;
}

void pushOrMergeOperationProcessus(ListeTQ liste, enum OperationProcessus type, int n) {

    if (!liste || !liste->tete) {
        inserQueueLTQ(liste, allocQuantum(type, n));
        return;
    }

    Liste last = liste->tete;
    while (last->suivant)
        last = last->suivant;

    Quantum *q = last->data;

    if (q->type == type) {
        q->nbQuantum += n;
    } else {
        inserQueueLTQ(liste, allocQuantum(type, n));
    }
}