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

    AlgoController& controller = AlgoController::getInstance();

    controller.setCSV((char*)filename);

    ExecutionTimeline* tl = AlgoController::selectAlgorithm(FIFO);

    if(!tl)
        return false;

    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);

    remove(filename);

    return true;
}

bool testAlgoControllerRRN() {

    const char* filename = "rrn_test.csv";

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

    AlgoController& controller = AlgoController::getInstance();

    controller.setCSV((char*)filename);

    ExecutionTimeline* tl = AlgoController::selectAlgorithm(RR);

    if(!tl)
        return false;

    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);

    remove(filename);

    return true;
}

bool testAlgoControllerSJF() {

    const char* filename = "sjf_test.csv";

    // Création d'un fichier CSV de test
    FILE* f = fopen(filename, "w");
    if (!f) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        return false;
    }

    // Nombre de processus
    fprintf(f,"8\n");

    // Format: nom;temps_arrivee;UC1;ES1;UC2;ES2;...
    fprintf(f,"P1;0;5;3;4\n");
    fprintf(f,"P2;0;2\n");
    fprintf(f,"P3;1;8\n");
    fprintf(f,"P4;2;1\n");
    fprintf(f,"P5;4;3;2;2\n");
    fprintf(f,"P6;6;4\n");
    fprintf(f,"P7;10;2;2;3\n");
    fprintf(f,"P8;35;5\n");

    /*fprintf(f,"P1;0;3;2;2;2;2\n");
    fprintf(f,"P2;1;2;2;3;3;2\n");*/

    fclose(f);

    // On initialise l'algorithme avec le CSV
    AlgoController& controller = AlgoController::getInstance();

    controller.setCSV((char*)filename);

    // Appel de SJF
    ExecutionTimeline* tl = AlgoController::selectAlgorithm(SJF);

    if (!tl) {
        std::cerr << "Erreur: Timeline SJF n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline SJF");
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    // Libération de la timeline
    destroyTimeline(tl);

    // Suppression du fichier CSV temporaire
    remove(filename);

    return true;
}

bool testAlgoControllerSJRF() {

    const char* filename = "sjrf_test.csv";

    // Création d'un fichier CSV de test
    FILE* f = fopen(filename, "w");
    if (!f) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        return false;
    }

    // Nombre de processus
    fprintf(f,"8\n");

    // Format: nom;temps_arrivee;UC1;ES1;UC2;ES2;...
    fprintf(f,"P1;0;5;3;4\n");
    fprintf(f,"P2;0;2\n");
    fprintf(f,"P3;1;8\n");
    fprintf(f,"P4;2;1\n");
    fprintf(f,"P5;4;3;2;2\n");
    fprintf(f,"P6;6;4\n");
    fprintf(f,"P7;10;2;2;3\n");
    fprintf(f,"P8;35;5\n");

    // exemple fin page pdf de jlb
    /*fprintf(f,"P1;0;3;4;2;3;3\n");
    fprintf(f,"P2;1;1;2;4;1;4\n");*/

    // exemple dans le discord AbsolutOrdor dans l'onglet Document
    /*fprintf(f,"P1;0;3;2;2;2;2\n");
    fprintf(f,"P2;1;2;2;3;3;2\n");*/


    fclose(f);

    // On initialise l'algorithme avec le CSV
    AlgoController& controller = AlgoController::getInstance();

    controller.setCSV((char*)filename);

    // Appel de SJF
    ExecutionTimeline* tl = AlgoController::selectAlgorithm(SJRF);

    if (!tl) {
        std::cerr << "Erreur: Timeline SJRF n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline SJRF");
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    // Libération de la timeline
    destroyTimeline(tl);

    // Suppression du fichier CSV temporaire
    remove(filename);

    return true;
}


bool testAlgoControllerLotteryScheduling() {

    const char* filename = "lottery_scheduling_test.csv";

    // Création d'un fichier CSV de test
    FILE* f = fopen(filename, "w");
    if (!f) {
        std::cerr << "Erreur lors de la création du fichier CSV" << std::endl;
        return false;
    }

    // Nombre de processus
    fprintf(f,"8\n");

    // Format: nom;temps_arrivee;UC1;ES1;UC2;ES2;...
    fprintf(f,"P1;0;5;3;4\n");
    fprintf(f,"P2;0;2\n");
    fprintf(f,"P3;1;8\n");
    fprintf(f,"P4;2;1\n");
    fprintf(f,"P5;4;3;2;2\n");
    fprintf(f,"P6;6;4\n");
    fprintf(f,"P7;10;2;2;3\n");
    fprintf(f,"P8;35;5\n");


    fclose(f);

    // On initialise l'algorithme avec le CSV
    AlgoController& controller = AlgoController::getInstance();

    controller.setCSV((char*)filename);

    // Appel de SJF
    ExecutionTimeline* tl = AlgoController::selectAlgorithm(LOTTERY);

    if (!tl) {
        std::cerr << "Erreur: Timeline Lottery Scheduling n'a pas pu être générée" << std::endl;
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline Lottery Scheduling");
    afficherTimeline(tl);
    afficherTimelineAvecDecalage(tl);

    // Libération de la timeline
    destroyTimeline(tl);

    // Suppression du fichier CSV temporaire
    remove(filename);

    return true;
}