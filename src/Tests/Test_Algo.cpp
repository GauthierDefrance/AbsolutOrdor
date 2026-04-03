#include <iostream>
#include <cstdio>
#include <fstream>
#include "process/AlgoController.h"
#include "../cli/ConsoleStructPrinter.h"

#define LOG_STEP(msg) std::cout << " [STEP] " << msg << std::endl;

static bool writeLinesToFile(const char* filename, const std::initializer_list<const char*>& lines)
{
    std::ofstream f(filename, std::ios::trunc);
    if (!f) return false;
    for (const char* line : lines) {
        f << line;
    }
    return true; // fermeture automatique à la fin du scope
}

bool testAlgoControllerFIFO() {
    const char* filename = "fifo_test.csv";

    // Écriture du fichier CSV (fermé avant lecture par AlgoController)
    if (!writeLinesToFile(filename, {
        "8\n",
        "P1;0;5;3;4\n",
        "P2;0;2\n",
        "P3;1;8\n",
        "P4;2;1\n",
        "P5;4;3;2;2\n",
        "P6;6;4\n",
        "P7;10;2;2;3\n",
        "P8;35;5\n"
    })) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        remove(filename);
        return false;
    }

    AlgoController& controller = AlgoController::getInstance();
    controller.setCSV((char*)filename);
    AlgoController::selectAlgorithm("FIFO");
    AlgoController::runAlgorithm();

    ExecutionTimeline* tl = AlgoController::getExecutionTimeline();
    if(!tl){
        std::cerr << "Erreur: Timeline FIFO n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);
    destroyTimeline(tl);
    remove(filename);
    return true;
}

bool testAlgoControllerRRN(int quantumRR = 2) {
    const char* filename = "rrn_test.csv";
    AlgoConfig config{};
    config.quantumRR = quantumRR;

    if (!writeLinesToFile(filename, {
        "8\n",
        "P1;0;5;3;4\n",
        "P2;1;2\n",
        "P3;2;8\n",
        "P4;2;1\n",
        "P5;4;3;2;2\n",
        "P6;6;4\n",
        "P7;10;2;2;3\n",
        "P8;35;5\n"
    })) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        remove(filename);
        return false;
    }

    AlgoController& controller = AlgoController::getInstance();
    controller.setCSV((char*)filename);
    AlgoController::selectAlgorithm("RR", config);
    AlgoController::runAlgorithm();

    ExecutionTimeline* tl = AlgoController::getExecutionTimeline();
    if(!tl){
        std::cerr << "Erreur: Timeline RR n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);
    destroyTimeline(tl);
    remove(filename);
    return true;
}

bool testAlgoControllerSJF() {
    const char* filename = "sjf_test.csv";

    if (!writeLinesToFile(filename, {
        "8\n",
        "P1;0;5;3;4\n",
        "P2;0;2\n",
        "P3;1;8\n",
        "P4;2;1\n",
        "P5;4;3;2;2\n",
        "P6;6;4\n",
        "P7;10;2;2;3\n",
        "P8;35;5\n"
    })) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        remove(filename);
        return false;
    }

    AlgoController& controller = AlgoController::getInstance();
    controller.setCSV((char*)filename);

    AlgoController::selectAlgorithm("SJF");
    AlgoController::runAlgorithm();

    ExecutionTimeline* tl = AlgoController::getExecutionTimeline();
    if (!tl) {
        std::cerr << "Erreur: Timeline SJF n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline SJF");
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);
    remove(filename);
    return true;
}

bool testAlgoControllerSJRF() {
    const char* filename = "sjrf_test.csv";

    if (!writeLinesToFile(filename, {
        "8\n",
        "P1;0;5;3;4\n",
        "P2;0;2\n",
        "P3;1;8\n",
        "P4;2;1\n",
        "P5;4;3;2;2\n",
        "P6;6;4\n",
        "P7;10;2;2;3\n",
        "P8;35;5\n"
    })) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        remove(filename);
        return false;
    }

    AlgoController& controller = AlgoController::getInstance();
    controller.setCSV((char*)filename);

    AlgoController::selectAlgorithm("SJRF");
    AlgoController::runAlgorithm();

    ExecutionTimeline* tl = AlgoController::getExecutionTimeline();
    if (!tl) {
        std::cerr << "Erreur: Timeline SJRF n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline SJRF");
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);
    remove(filename);
    return true;
}

bool testAlgoControllerLotteryScheduling() {
    const char* filename = "lottery_scheduling_test.csv";

    if (!writeLinesToFile(filename, {
        "8\n",
        "P1;0;5;3;4\n",
        "P2;0;2\n",
        "P3;1;8\n",
        "P4;2;1\n",
        "P5;4;3;2;2\n",
        "P6;6;4\n",
        "P7;10;2;2;3\n",
        "P8;35;5\n"
    })) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        remove(filename);
        return false;
    }

    AlgoController& controller = AlgoController::getInstance();
    controller.setCSV((char*)filename);

    AlgoController::selectAlgorithm("LOTTERY");
    AlgoController::runAlgorithm();

    ExecutionTimeline* tl = AlgoController::getExecutionTimeline();
    if (!tl) {
        std::cerr << "Erreur: Timeline Lottery Scheduling n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline Lottery Scheduling");
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);
    remove(filename);
    return true;
}