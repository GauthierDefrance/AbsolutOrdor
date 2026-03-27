#include "MainCli.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>

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
        "  ordor -i \"<contenu csv>\" -s <ALGO>\n"
        "  ordor -f <fichier.csv> -s all\n"
        "  ordor -f <fichier.csv> -s FIFO -o <sortie.csv>\n"
        "\n"
        "OPTIONS\n"
        "  -f, -filepath <path>    Fichier CSV des processus\n"
        "  -i, -inline   <str>     Contenu CSV passe directement en argument\n"
        "  -s, -select   <algo>    Algorithme a utiliser (ex: FIFO, SJF, all pour tous)\n"
        "  -n <quantum>            Quantum si vous utilisez RR (defaut: 2)\n"
        "  -o, -output   <path>    Fichier de sortie CSV des resultats\n"
        "  -h, -help               Affiche ce message\n"
        "\n"
        "EXEMPLES\n"
        "  ordor -f processus.csv -s FIFO\n"
        "  ordor -f processus.csv -s RR -n 3 -o resultats.csv\n"
        "  ordor -i \"2\\nP1;0;5;3\\nP2;2;3\" -s SJF (Pour un csv inline)\n"
        "  ordor -f processus.csv -s all -o resultats.csv\n"
        "\n";
    printBuiltinAlgorithm();
}

void MainCli::printBuiltinAlgorithm() {
    std::cout <<
        "ALGORITHMES\n"
        "  FIFO      Premier arrive, premier servi\n"
        "  SJF       Plus court job en premier\n"
        "  SJRF      Plus court job restant\n"
        "  RR        Round Robin  (parametre -n X requis)\n"
        "  LOTTERY   Lottery Scheduling\n"
        "\n";
}

void MainCli::loadFile(char *filepath) {
    AlgoController::getInstance().setCSV(filepath);
}

void MainCli::printCurrentFileName() {
    std::string name = AlgoController::getInstance().getCurrentCSVName();
    std::string path = AlgoController::getInstance().getCurrentCSVPath();

    if (!name.empty())
        std::cout << "Nom fichier courant : "    << name << "\n"
                  << "Chemin fichier courant : " << path << "\n";
    else
        std::cout << "Source : inline CSV\n";
}

void MainCli::selectAlgorithm(const char* algorithm, AlgoConfig config, const std::string& outputPath) {
    ExecutionTimeline *tl = AlgoController::selectAlgorithm(algorithm, config);
    if (!tl) {
        std::cerr << "Erreur : impossible de generer la timeline pour " << algorithm << "\n";
        return;
    }
    afficherTimelineAvecDecalage(tl);
    afficherStatsTimeline(tl);

    if (!outputPath.empty())
        exportStatsCSV(tl, algorithm, config, outputPath, false);

    destroyTimeline(tl);
}

void MainCli::loadFileAndSelectAlgorithm(char *filepath, const char* algorithm, AlgoConfig config) {
    loadFile(filepath);
    selectAlgorithm(algorithm, config);
}

void MainCli::exportStatsCSV(const ExecutionTimeline *tl, const char* algo, AlgoConfig config, const std::string& outputPath, bool append) {

    bool fileExists = false;
    {
        FILE* check = fopen(outputPath.c_str(), "r");
        if (check) { fileExists = true; fclose(check); }
    }

    FILE* out = fopen(outputPath.c_str(), append ? "a" : "w");
    if (!out) {
        std::cerr << "Erreur : impossible d'ecrire dans " << outputPath << "\n";
        return;
    }

    if (!append || !fileExists) {
        fprintf(out, "Algorithme;QuantumRR;NbProcessus;Debut;Fin;DureeTotale;"
                     "TicksUC;TicksES;TicksWait;"
                     "AttenteMoy;RestitutionMoy;ReponseMoy;TauxCPU\n");
    }

    double attenteMoy     = attenteMoyProcessus((ExecutionTimeline*)tl);
    double restitutionMoy = restitutionMoyProcessus((ExecutionTimeline*)tl);
    double reponseMoy     = tempRepMoyProcessus((ExecutionTimeline*)tl);
    double cpuUtil        = tauxOccupationCPU((ExecutionTimeline*)tl);
    int tDebut  = getTimelineDebut(tl);
    int tMax    = getTimelineMax(tl);
    int nbProc  = getTimelineNbProcessus(tl);
    int ticksUC = getTimelineTicksType(tl, UC);
    int ticksES = getTimelineTicksType(tl, ES);
    int ticksW  = getTimelineTicksType(tl, W);

    fprintf(out, "%s;%d;%d;%d;%d;%d;%d;%d;%d;%.2f;%.2f;%.2f;%.2f\n",
            algo,
            config.quantumRR,
            nbProc, tDebut, tMax, tMax - tDebut,
            ticksUC, ticksES, ticksW,
            attenteMoy, restitutionMoy, reponseMoy, cpuUtil);

    fprintf(out, "\n");
    fprintf(out, "Algorithme;Processus;Arrivee;TicksUC;TicksES;TicksWait;Restitution;Reponse\n");

    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus *) cell->data;

        int pUC = 0, pES = 0, pW = 0;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            Quantum *q = (Quantum *) qc->data;
            if      (q->type == UC) pUC += q->nbQuantum;
            else if (q->type == ES) pES += q->nbQuantum;
            else                    pW  += q->nbQuantum;
        }

        int restitution = tempsRestitutionProcessus(p);
        int reponse     = tempsRepProcessus(p);

        fprintf(out, "%s;%s;%d;%d;%d;%d;%d;%d\n",
                algo,
                p->name, p->timeArrival,
                pUC, pES, pW,
                restitution, reponse);
    }

    fprintf(out, "\n");
    fclose(out);
    std::cout << "Resultats exportes dans : " << outputPath << "\n";
}

int MainCli::run(int argc, char** argv) {

    if (argc == 1) {
        std::cerr << "Erreur : Aucun argument fourni.\n\n";
        showHelp();
        return 1;
    }

    char* filepath      = nullptr;
    std::string inlineCSV;
    bool hasInline      = false;
    bool runAll         = false;
    std::string selectedAlgoStr;
    bool algoSelected   = false;
    AlgoConfig config   = {};
    std::string outputPath;

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
        else if (arg == "-i" || arg == "-inline") {
            if (i + 1 < argc) {
                inlineCSV = argv[++i];
                hasInline = true;
            } else {
                std::cerr << "Erreur : Contenu CSV manquant apres -i.\n";
                return 1;
            }
        }
        else if (arg == "-o" || arg == "-output") {
            if (i + 1 < argc) {
                outputPath = argv[++i];
            } else {
                std::cerr << "Erreur : Chemin de sortie manquant.\n";
                return 1;
            }
        }
        else if (arg == "-n") {
            if (i + 1 < argc) {
                config.quantumRR = std::stoi(argv[++i]);
                if (config.quantumRR <= 0) {
                    std::cerr << "Erreur : Le quantum RR doit etre > 0.\n";
                    return 1;
                }
            } else {
                std::cerr << "Erreur : Valeur du quantum RR manquante.\n";
                return 1;
            }
        }
        else if (arg == "-s" || arg == "-select") {
            if (i + 1 < argc) {
                std::string algo = argv[++i];

                if (algo == "all") {
                    runAll = true;
                    continue;
                }

                // Validation de l'algo
                if (algo == "FIFO" || algo == "SJF"  ||
                    algo == "SJRF" || algo == "RR"   ||
                    algo == "LOTTERY") {
                    selectedAlgoStr = algo;
                    algoSelected    = true;
                } else {
                    std::cerr << "Erreur : Algorithme inconnu : " << algo << "\n\n";
                    printBuiltinAlgorithm();
                    return 1;
                }
            } else {
                std::cerr << "Erreur : Algorithme manquant apres -s.\n";
                return 1;
            }
        }
        else {
            std::cerr << "Erreur : Argument inconnu : " << arg << "\n\n";
            showHelp();
            return 1;
        }
    }

    // Vérification source
    if (filepath == nullptr && !hasInline) {
        std::cerr << "Erreur : Aucune source fournie (-f ou -i).\n";
        return 1;
    }
    if (filepath != nullptr && hasInline) {
        std::cerr << "Erreur : -f et -i sont mutuellement exclusifs.\n";
        return 1;
    }

    // Chargement
    if (hasInline) {
        AlgoController::getInstance().setCSV(inlineCSV);
    } else {
        loadFile(filepath);
    }
    printCurrentFileName();

    // Effacer le fichier de sortie avant le mode "all"
    if (!outputPath.empty() && runAll) {
        FILE* clr = fopen(outputPath.c_str(), "w");
        if (clr) fclose(clr);
    }

    if (runAll) {
        std::cout << "\nExecution de tous les algorithmes...\n";
        const char* noms[] = {"FIFO", "SJF", "SJRF", "RR", "LOTTERY"};
        int nbAlgos = sizeof(noms) / sizeof(noms[0]);

        for (int i = 0; i < nbAlgos; i++) {
            std::cout << "\n=== Algorithme : " << noms[i] << " ===\n";
            ExecutionTimeline *tl = AlgoController::selectAlgorithm(noms[i], config);
            if (!tl) { std::cerr << "Erreur allocation pour " << noms[i] << "\n"; continue; }
            afficherTimelineAvecDecalage(tl);
            afficherStatsTimeline(tl);
            if (!outputPath.empty())
                exportStatsCSV(tl, noms[i], config, outputPath, i > 0);
            destroyTimeline(tl);
        }

    } else if (algoSelected) {
        selectAlgorithm(selectedAlgoStr.c_str(), config, outputPath);

    } else {
        std::cerr << "Erreur : Aucun algorithme selectionne.\n\n";
        printBuiltinAlgorithm();
        return 1;
    }

    return 0;
}