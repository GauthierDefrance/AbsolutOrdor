#include <iostream>

extern "C" {
#include "data/struct/liste_tq.h"
}

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testListeTQ() {

    LOG_STEP("Allocation ListeTQ");

    ListeTQ ltq = allocMemLTQ();
    initLTQ(ltq);

    int *x = (int*)malloc(sizeof(int));
    int *y = (int*)malloc(sizeof(int));

    *x = 100;
    *y = 200;

    inserQueueLTQ(ltq, x);
    inserQueueLTQ(ltq, y);

    if (*(int*)donneeListe(teteLTQ(ltq)) != 100)
        return false;

    if (*(int*)donneeListe(queueLTQ(ltq)) != 200)
        return false;

    LOG_STEP("Suppression queue");

    suppQueueLTQ(ltq, free);

    if (*(int*)donneeListe(queueLTQ(ltq)) != 100)
        return false;

    destroyLTQ(ltq, free);

    return true;
}