#pragma once
#include "stdio.h"
#include "process/AlgoController.h"
#include "ConsoleStructPrinter.h"

extern "C" {
#include "data/struct/liste.h"
#include "data/struct/liste_tq.h"
#include "data/struct/processus.h"
#include "data/struct/ExecutionTimeline.h"
#include "process/utilities/csv_reader.h"
#include "process/utilities/TimelineStatsCalculator.h"
}

#include <string>

struct Fichier {
    FILE *fp;
    char *path;
};

class MainCli {

public:
    static MainCli& getInstance();
    ~MainCli();

    int run(int argc, char** argv);

    static void showHelp();
    static void printBuiltinAlgorithm();

    void loadFile(char *filepath);
    void printCurrentFileName();

    void selectAlgorithm(const char* algorithm,AlgoConfig config = {},const std::string& outputPath = "");

    void loadFileAndSelectAlgorithm(char *filepath,const char* algorithm,AlgoConfig config = {});

    void exportStatsCSV(const ExecutionTimeline *tl,const char* algo,AlgoConfig config,const std::string& outputPath,bool append = false);

private:
    MainCli();
    Fichier *f = nullptr;
};