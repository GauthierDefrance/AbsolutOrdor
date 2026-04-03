#include <iostream>

extern "C" {
#include "data/struct/ExecutionTimeline.h"
}

#include "../cli/ConsoleStructPrinter.h"

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testExecutionTimeline() {

    LOG_STEP("Creation timeline");

    ExecutionTimeline *tl = allocTimeline();

    if(!tl)
        return false;

    afficherTimeline(tl);

    destroyTimeline(tl);

    return true;
}