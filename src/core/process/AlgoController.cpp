#include "AlgoController.h"
#include "process/utilities/csv_reader.h"

#include "Algo/sjf.h"


TabProcessus AlgoController::tabProcessus = nullptr;

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
    if (AlgoController::tabProcessus != nullptr) {
        freeTabProcessus(&AlgoController::tabProcessus);
    }
    AlgoController::tabProcessus = createTabProcessusFromCSV(sourcepath);
}


/**
 * Cette méthode, permet de sélectionner un algorithme et généré son résultat.
 * Elle nécessite qu'un CSV est d'abord était chargé précédemment.
 *
 * @param algorithm, l'algorithme que vous voulez utiliser pour l'ordonnancement
 * @return Resultat* qui pourra être ensuite affiché
 */
Resultat* AlgoController::selectAlgorithm(SchedulingAlgorithm algorithm) {

    if (AlgoController::tabProcessus== nullptr) { return nullptr; }

    switch (algorithm) {
        case FIFO : {
            return nullptr;
        }
        case SJF : {
            return sjf(AlgoController::tabProcessus->tabProcess, AlgoController::tabProcessus->nbProcess);
        }
        case SJRF : {
            return nullptr;
        }
        case RR : {
            return nullptr;
        }
        default: {
            return nullptr;
        }

    }
}