#include "AlgoController.h"
#include "process/utilities/csv_reader.h"

#include "Algo/sjf.h"


/**
 * Renvoie l'instance unique d'AlgoController
 * @return INSTANCE AlgoController
 */
AlgoController& AlgoController::getInstance() {
    static AlgoController instance;
    return instance;
}


/**
 * Destructeur de l'objet AlgoController, libération de la mémoire.
 */
AlgoController::~AlgoController() {
    if (tabProcessus != NULL) {
        freeTabProcessus(&tabProcessus);
    }
}


/**
 * Permet de lire un CSV et générer le tableau de Processus associé.
 * @param sourcepath
 */
void AlgoController::setCSV(char *sourcepath) {
    if (tabProcessus != NULL) {
        freeTabProcessus(&tabProcessus);
    }
    tabProcessus = createTabProcessusFromCSV(sourcepath);
}


/**
 * Cette méthode, permet de sélectionner un algorithme et généré son résultat.
 * Elle nécessite qu'un CSV est d'abord était chargé précédemment.
 *
 * @param algorithm, l'algorithme que vous voulez utiliser pour l'ordonnancement
 * @return Resultat* qui pourra être ensuite affiché
 */
Resultat* AlgoController::selectAlgorithm(SchedulingAlgorithm algorithm) {

    if (tabProcessus== NULL) { return nullptr; }

    switch (algorithm) {
        case FIFO : {
            //return fifo();
        }
        case SJF : {
            return sjf(tabProcessus->tabProcess, tabProcessus->nbProcess);
        }
        case SJRF : {
            //return sjrf();
        }
        case RR : {
            //return rr();
        }
        default: {
            return nullptr;
        }

    }
}