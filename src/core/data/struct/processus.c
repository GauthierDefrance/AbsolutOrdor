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
