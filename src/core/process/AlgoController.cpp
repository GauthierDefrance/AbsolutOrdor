#include "AlgoController.h"
#include <iostream>

// Liste générique de Processus* lue depuis le CSV
ListeTQ     AlgoController::listeProcessus = nullptr;
std::string AlgoController::currentCSVPath = "";
std::string AlgoController::currentCSVName = "";

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
    currentCSVPath = "";
    currentCSVName = "";
}

/**
 * Charge un CSV et génère la liste de Processus associée.
 */

void AlgoController::setCSV(char *sourcepath) {

    if (AlgoController::listeProcessus != nullptr) {
        destroyLTQ(AlgoController::listeProcessus, (void (*)(void *)) libMemProcessus);
        AlgoController::listeProcessus = nullptr;
    }

    AlgoController::listeProcessus = createListeProcessusFromCSV(sourcepath);

    if (!isListeProcessusValid(AlgoController::listeProcessus)) {
        std::cerr << "[AlgoController] Erreur : liste de processus invalide." << std::endl;
        destroyLTQ(AlgoController::listeProcessus, (void (*)(void *)) libMemProcessus);
        AlgoController::listeProcessus = nullptr;

        // reset aussi les infos fichier
        currentCSVPath = "";
        currentCSVName = "";
        return;
    }

    // 🔽 Stocker le path
    currentCSVPath = sourcepath;

    // 🔽 Extraire le nom du fichier
    std::string pathStr(sourcepath);
    size_t pos = pathStr.find_last_of("/\\");
    if (pos != std::string::npos) {
        currentCSVName = pathStr.substr(pos + 1);
    } else {
        currentCSVName = pathStr;
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
            return sjf(listeProcessus, tailleListe(listeProcessus->tete));
        }
        case SJRF: {
            return sjrf(listeProcessus, tailleListe(listeProcessus->tete));
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

std::string AlgoController::getCurrentCSVPath() {
    return currentCSVPath;
}

std::string AlgoController::getCurrentCSVName() {
    return currentCSVName;
}