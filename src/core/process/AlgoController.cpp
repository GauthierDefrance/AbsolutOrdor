#include "AlgoController.h"



#include <iostream>

// Liste générique de Processus* lue depuis le CSV
ListeTQ AlgoController::listeProcessus = nullptr;

/**
 * Renvoie l'instance unique d'AlgoController
 */
AlgoController& AlgoController::getInstance() {
    static AlgoController instance;
    return instance;
}

/**
 * Destructeur : libère la liste de processus si elle existe.
 */
AlgoController::~AlgoController() {
    if (listeProcessus != nullptr) {
        // Libère tous les Processus* contenus dans la liste
        destroyLTQ(listeProcessus, (void (*)(void *)) libMemProcessus);
        listeProcessus = nullptr;
    }
}

/**
 * Charge un CSV et génère la liste de Processus associée.
 */
void AlgoController::setCSV(char *sourcepath) {
    // On libère l’ancienne liste si elle existe
    if (AlgoController::listeProcessus != nullptr) {
        destroyLTQ(AlgoController::listeProcessus, (void (*)(void *)) libMemProcessus);
        AlgoController::listeProcessus = nullptr;
    }

    AlgoController::listeProcessus = createListeProcessusFromCSV(sourcepath);

    if (!isListeProcessusValid(AlgoController::listeProcessus)) {
        std::cerr << "[AlgoController] Erreur : liste de processus invalide après lecture du CSV." << std::endl;
        destroyLTQ(AlgoController::listeProcessus, (void (*)(void *)) libMemProcessus);
        AlgoController::listeProcessus = nullptr;
    }
}

/**
 * Sélectionne un algorithme et génère sa timeline d’exécution.
 * Nécessite qu’un CSV ait été chargé avant.
 */
ExecutionTimeline* AlgoController::selectAlgorithm(SchedulingAlgorithm algorithm) {
    if (AlgoController::listeProcessus == nullptr) {
        std::cerr << "[AlgoController] Aucun CSV chargé, impossible de lancer un algorithme." << std::endl;
        return nullptr;
    }

    switch (algorithm) {
        case FIFO: {
            return fifo(listeProcessus);
        }
        case SJF: {
            // Signature attendue côté C :
            // ExecutionTimeline* sjf(ListeTQ listeProcessus);
            //return sjf(AlgoController::listeProcessus);
            return nullptr;
        }
        case SJRF: {
            // TODO: implémenter SJRF(listeProcessus)
            std::cerr << "[AlgoController] SJRF non encore implémenté." << std::endl;
            return nullptr;
        }
        case RR: {
            // TODO: implémenter RR(listeProcessus)
            std::cerr << "[AlgoController] RR non encore implémenté." << std::endl;
            return nullptr;
        }
        default: {
            std::cerr << "[AlgoController] Algorithme inconnu." << std::endl;
            return nullptr;
        }
    }
}
