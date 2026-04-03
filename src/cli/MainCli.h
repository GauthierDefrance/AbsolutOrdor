#pragma once
#include "process/AlgoController.h"
#include "ConsoleStructPrinter.h"
#include "process/utilities/csv_writer.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>

extern "C" {
    #include "data/struct/liste.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/processus.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/utilities/TimelineStatsCalculator.h"
}


/**
 * Classe servant à controller AlgoController via le terminal.
 * Elle gère quelques commandes et paramètres et utilise AlgoController
 * pour répondre aux demandes de l'utilisateur.
 */
class MainCli {
    public:
        /**
         * @return le singleton MainCli
         */
        static MainCli& getInstance();

        // Destructeur de classe
        ~MainCli();

        /**
         * Fonction servant à démarrer le MainCLI
         * @param argc
         * @param argv
         * @return un code indiquant l'état à la fin du code
         */
        int run(int argc, char** argv);


        /**
         * Affiche dans la console un message d'aide
         */
        static void showHelp();


        /**
         * Affiche la liste des algorithmes implémentés
         */
        static void printBuiltinAlgorithm();


        /**
         * Affiche les informations sur le fichier sauvegardé temporairement dans AlgoController
         */
        void printCurrentFileName();

    private:
        // Constructeur de classe
        MainCli();
};