#include <iostream>
#include <cstring>
#include <cstdio>

extern "C" {
#include "data/struct/processus.h"
}

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testProcessus() {

    LOG_STEP("Creation processus");

    Processus *p = allocMemProcessus();
    initProcessus(p);

    std::snprintf(p->name, NBMAXCHAR, "%s", "TestProc");
    p->timeArrival = 5;

    Quantum *q1 = (Quantum*)malloc(sizeof(Quantum));
    Quantum *q2 = (Quantum*)malloc(sizeof(Quantum));

    q1->type = UC;
    q1->nbQuantum = 3;

    q2->type = ES;
    q2->nbQuantum = 7;

    inserQueueLTQ(p->listeTQ, q1);
    inserQueueLTQ(p->listeTQ, q2);

    if (strcmp(processusName(p), "TestProc") != 0)
        return false;

    if (getTimeArrival(p) != 5)
        return false;

    libMemProcessus(p);

    return true;
}