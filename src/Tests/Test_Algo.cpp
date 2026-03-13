#include <iostream>
#include <cstdio>

extern "C" {
#include "process/utilities/csv_reader.h"
}

#include "process/AlgoController.h"
#include "../cli/ConsoleStructPrinter.h"

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testAlgoControllerFIFO() {

    const char* filename = "fifo_test.csv";

    FILE* f = fopen(filename,"w");

    fprintf(f,"8\n");

    fprintf(f,"P1;0;5;3;4\n");
    fprintf(f,"P2;0;2\n");
    fprintf(f,"P3;1;8\n");
    fprintf(f,"P4;2;1\n");
    fprintf(f,"P5;4;3;2;2\n");
    fprintf(f,"P6;6;4\n");
    fprintf(f,"P7;10;2;2;3\n");
    fprintf(f,"P8;35;5\n");

    fclose(f);

    AlgoController::setCSV((char*)filename);

    ExecutionTimeline* tl = AlgoController::selectAlgorithm(FIFO);

    if(!tl)
        return false;

    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);

    remove(filename);

    return true;
}