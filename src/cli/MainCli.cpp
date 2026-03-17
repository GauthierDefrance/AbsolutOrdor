#include "MainCli.h"
#include "stdio.h"


MainCli& MainCli::getInstance() {
        static MainCli instance;
        return instance;
}

MainCli::MainCli() {

}

MainCli::~MainCli() {
        delete f;
}


// Etat du CLI
void MainCli::run(int argc, char** argv) {

}

//Fonctions d'affichages
void MainCli::showHelp() {


}
void MainCli::printBuiltinAlgorithm() {

}

//static void printAlgorithmResultat(Resultat resultat);

//Méthodes utilitaire
void MainCli::loadFile(char *filepath) {

}


void MainCli::printCurrentFileName() {

}


void MainCli::selectAlgorithm(SchedulingAlgorithm algorithm) {
        

}


void MainCli::loadFileAndSelectAlgorithm(char *filepath, SchedulingAlgorithm algorithm) {

}