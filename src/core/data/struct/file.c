#include "file.h"
#include <stdlib.h>
#include <stdio.h>


/*******************************************
    Allocation / Initialisation
********************************************/
File allocFile() {
    File f = malloc(sizeof(struct File_Cellule));
    if (!f) {
        fprintf(stderr, "Erreur : allocation File\n");
        return NULL;
    }

    f->ltq = allocMemLTQ();
    if (!f->ltq) {
        free(f);
        return NULL;
    }

    initLTQ(f->ltq);
    return f;
}

void initFile(File f) {
    if (!f) return;
    initLTQ(f->ltq);
}


/*******************************************
    Opérations principales
********************************************/
void enfilerFile(File f, void *data) {
    if (!f) return;
    inserQueueLTQ(f->ltq, data);   // FIFO : on ajoute en queue
}

/**
 * Fonction qui permet de défiler une File
 * WARNING : Vous êtes responsable de la libération mémoire, cette fonction ne le fait pas pour vous
 * @param f
 * @return
 */
void *defilerFile(File f) {
    if (!f || estVideFile(f)) return NULL;

    Liste tete = f->ltq->tete;
    void *data = tete->data;

    // suppression sans libérer data
    suppTeteLTQ(f->ltq, NULL);

    return data;
}

void *teteFile(File f) {
    if (!f || estVideFile(f)) return NULL;
    return f->ltq->tete->data;
}

bool estVideFile(File f) {
    return (!f || f->ltq->tete == NULL);
}


int tailleFile(File f) {
    // Vérification de sécurité : si la file ou la liste interne n'existe pas
    if (!f || !f->ltq) {
        return 0;
    }

    // On délègue le calcul à la fonction tailleListe existante
    // en passant la tête de liste contenue dans le ListeTQ
    return tailleListe(f->ltq->tete);
}

/*******************************************
    Destruction
********************************************/
void destroyFile(File f, void (*freeData)(void *)) {
    if (!f) return;

    destroyLTQ(f->ltq, freeData);
    free(f);
}
