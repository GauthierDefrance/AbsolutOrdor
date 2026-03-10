#pragma once
#include "process/AlgoController.h"


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
        void run(int argc, char** argv); // The reading of the input from the user loop and reacting to it


        //Fonctions d'affichages
        static void showHelp();
        static void printBuiltinAlgorithm();
        static void printAlgorithmResultat(Resultat resultat);

        //Méthodes utilitaire
        void loadFile(char *filepath);
        void printCurrentFileName();
        void selectAlgorithm(SchedulingAlgorithm algorithm);


    private:
        MainCli();

        Fichier *f = nullptr;
        Resultat resultat;
        static MainCli* INSTANCE;

};

