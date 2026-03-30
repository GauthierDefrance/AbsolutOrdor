#pragma once
#include <cstdio>
#include "process/AlgoController.h"
#include "ConsoleStructPrinter.h"
#include <string>

extern "C" {
    #include "data/struct/liste.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/processus.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/utilities/csv_reader.h"
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


        /**
         * Génère un CSV avec les résultats et statitisques
         * @param tl La timeline source
         * @param algo pour quel algo
         * @param config les paramètres particuliers tel que "-n"
         * @param outputPath le nom du fichier dans lequel exporter le résultat
         * @param append ajouter à la fin du fichier ou non
         */
        void exportStatsCSV(const ExecutionTimeline *tl,const char* algo, AlgoConfig config, const std::string& outputPath, bool append = false);

    private:
        // Constructeur de classe
        MainCli();
};