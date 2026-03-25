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

}



struct Fichier {
    FILE *fp; // Le contenu du fichier
    char *path; //Son chemin
};



class MainCli {

    public:
        //Constructeur et destructeur
        static MainCli& getInstance();
        ~MainCli();


        // Etat du CLI
        int run(int argc, char** argv); // The reading of the input from the user loop and reacting to it


        //Fonctions d'affichages
        static void showHelp();
        static void printBuiltinAlgorithm();
        //static void printAlgorithmResultat(Resultat resultat);

        //Méthodes utilitaire
        void loadFile(char *filepath);
        void printCurrentFileName();
        void selectAlgorithm(SchedulingAlgorithm algorithm, AlgoConfig config = {});
        void loadFileAndSelectAlgorithm(char *filepath, SchedulingAlgorithm algorithm, AlgoConfig config = {});


    private:
        MainCli();

        Fichier *f = nullptr;
        //Resultat resultat;
        static MainCli* INSTANCE;

};

