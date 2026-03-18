#include "MainCli.h"
#include <iostream>
#include <vector>

MainCli& MainCli::getInstance() {
    static MainCli instance;
    return instance;
}

MainCli::MainCli() {}

MainCli::~MainCli() {
    delete f;
}

void MainCli::showHelp() {
    std::cout <<
        "\n"
        "  ABSOLUT ORDOR\n"
        "  Simulateur d'ordonnancement CPU\n"
        "\n"
        "UTILISATION\n"
        "  ordor -f <fichier.csv> -s <ALGO>\n"
        "  ordor -f <fichier.csv> -s all\n"
        "\n"
        "OPTIONS\n"
        "  -f, -filepath <path>    Fichier CSV des processus\n"
        "  -s, -select   <algo>    Algorithme a utiliser  (ex: FIFO, SJF, * pour tous)\n"
        "  -h, -help               Affiche ce message\n"
        "\n"
        "EXEMPLES\n"
        "  ordor -f processus.csv -s FIFO\n"
        "  ordor -f processus.csv -s SJF\n"
        "  ordor -f processus.csv -s *\n"
        "\n";
    printBuiltinAlgorithm();
}

void MainCli::printBuiltinAlgorithm() {
    std::cout <<
        "ALGORITHMES\n"
        " - FIFO\n"
        " - SJF\n"
        " - SJRF\n"
        " - RR\n"
        "\n";
}

void MainCli::loadFile(char *filepath) {
    AlgoController::getInstance().setCSV(filepath);
}

void MainCli::printCurrentFileName() {
    std::cout << "Nom fichier courant : "    << AlgoController::getInstance().getCurrentCSVName() << "\n"
              << "Chemin fichier courant : " << AlgoController::getInstance().getCurrentCSVPath() << "\n";
}

void MainCli::selectAlgorithm(SchedulingAlgorithm algorithm) {
    ExecutionTimeline *tl = AlgoController::selectAlgorithm(algorithm);
    if (!tl) {
        std::cerr << "Erreur critique, erreur d'allocation memoire.\n";
        return;
    }
    afficherTimelineAvecDecalage(tl);
    destroyTimeline(tl);
}

void MainCli::loadFileAndSelectAlgorithm(char *filepath, SchedulingAlgorithm algorithm) {
    loadFile(filepath);
    selectAlgorithm(algorithm);
}

int MainCli::run(int argc, char** argv) {

    if (argc == 1) {
        std::cerr << "Erreur : Aucun argument fourni.\n\n";
        showHelp();
        return 1;
    }

    char* filepath = nullptr;
    bool runAll = false;
    SchedulingAlgorithm selectedAlgo = {};
    bool algoSelected = false;

    for (int i = 1; i < argc; i++) {

        std::string arg = argv[i];

        if (arg == "-h" || arg == "-help") {
            showHelp();
            return 0;
        }
        else if (arg == "-f" || arg == "-filepath") {
            if (i + 1 < argc) {
                filepath = argv[++i];
            } else {
                std::cerr << "Erreur : Chemin de fichier manquant.\n";
                return 1;
            }
        }
        else if (arg == "-s" || arg == "-select") {
            if (i + 1 < argc) {
                std::string algo = argv[++i];
                int algoCode = -1;

                if (algo == "all") {
                    runAll = true;
                    continue;
                }
                else if (algo == "FIFO") algoCode = FIFO;
                else if (algo == "SJF")  algoCode = SJF;
                else if (algo == "SJRF") algoCode = SJRF;
                else if (algo == "RR")   algoCode = RR;

                switch (algoCode) {
                    case FIFO:
                    case SJF:
                    case SJRF:
                    case RR:
                        selectedAlgo = static_cast<SchedulingAlgorithm>(algoCode);
                        algoSelected = true;
                        break;
                    default:
                        std::cerr << "Erreur : Algorithme inconnu : " << algo << "\n\n";
                        printBuiltinAlgorithm();
                        return 1;
                }
            } else {
                std::cerr << "Erreur : Algorithme manquant.\n";
                return 1;
            }
        }
        else {
            std::cerr << "Erreur : Argument inconnu : " << arg << "\n\n";
            showHelp();
            return 1;
        }
    }

    if (filepath == nullptr) {
        std::cerr << "Erreur : Aucun fichier fourni.\n";
        return 1;
    }

    loadFile(filepath);
    printCurrentFileName();

    if (runAll) {
        std::cout << "\nExecution de tous les algorithmes...\n";
        std::vector<SchedulingAlgorithm> algos = {FIFO, SJF, SJRF};
        const char* noms[] = {"FIFO", "SJF", "SJRF"};
        int i = 0;
        for (auto algo : algos) {
            std::cout << "\n=== Algorithme : " << noms[i] << " ===\n";
            selectAlgorithm(algo);
            i++;
        }
    } else if (algoSelected) {
        selectAlgorithm(selectedAlgo);
    } else {
        std::cerr << "Erreur : Aucun algorithme selectionne.\n\n";
        printBuiltinAlgorithm();
        return 1;
    }

    return 0;
}