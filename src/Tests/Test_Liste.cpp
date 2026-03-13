#include <iostream>

extern "C" {
    #include "data/struct/liste.h"
}

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testListe() {

    LOG_STEP("Creation liste simple et insertion de 3 entiers");

    Liste l = nullptr;

    int *a = (int*)malloc(sizeof(int));
    int *b = (int*)malloc(sizeof(int));
    int *c = (int*)malloc(sizeof(int));

    *a = 10;
    *b = 20;
    *c = 30;

    inserTete(&l, a);
    inserTete(&l, b);
    inserQueue(&l, c);

    if (*(int*)donneeListe(l) != 20) return false;
    if (*(int*)donneeListe(suivantListe(l)) != 10) return false;
    if (*(int*)donneeListe(queueListe(l)) != 30) return false;

    LOG_STEP("Suppression de la tete");

    suppTete(&l, free);

    if (*(int*)donneeListe(l) != 10)
        return false;

    LOG_STEP("Destruction liste");

    destroyListe(l, free);

    return true;
}