/**
 * @file AlgoController.cpp
 * @brief Implémentation du contrôleur de logique métier.
 *
 * Ce module centralise les données de simulation. Il utilise des membres statiques
 * pour garantir que les données persistent tant qu'un nouveau fichier n'est pas chargé,
 * tout en offrant une interface simple pour l'interface graphique ou la console.
 */


#include "AlgoController.h"

#include <array>
#include <iostream>

#include "utilities/string_parser.h"


// Initialisation des membres statiques (obligatoire hors de la classe)
ListeTQ AlgoController::listeProcessus = nullptr;
std::string AlgoController::currentCSVPath = "";
std::string AlgoController::currentCSVName = "";
std::string algorithm_choice;
AlgoConfig algorithm_config = AlgoConfig(4);
ExecutionTimeline* execution_timeline = nullptr;


/**
 * @brief Implémentation du Singleton de type "Meyers".
 *
 * Garantit une instanciation unique et une destruction propre à la fin du programme.
 * @return L'instance unique d'AlgoController.
 */
AlgoController& AlgoController::getInstance() {
    static AlgoController instance;
    return instance;
}

/**
 * @brief Nettoie les ressources mémoire.
 *
 * Appelle explicitement les fonctions de libération C (`destroyLTQ` et `libMemProcessus`)
 * pour éviter toute fuite de mémoire sur les structures allouées dynamiquement.
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
 * @brief Dispatcher d'algorithme.
 *
 * Agit comme une fabrique (factory) qui choisit l'algorithme à exécuter en fonction
 * du choix utilisateur. Elle transmet la liste de processus actuelle aux fonctions C.
 *
 * @return ExecutionTimeline* Pointeur vers l'historique d'exécution généré.
 */
void AlgoController::runAlgorithm() {
    execution_timeline = nullptr;
    if ("FIFO" == algorithm_choice){execution_timeline = fifo(listeProcessus);}
    if ("SJF" == algorithm_choice) {execution_timeline = sjf(listeProcessus, tailleListe(listeProcessus->tete));}
    if ("SJRF" == algorithm_choice) {execution_timeline = sjrf(listeProcessus, tailleListe(listeProcessus->tete));}
    if ("RR" == algorithm_choice) {execution_timeline = rrn(listeProcessus, algorithm_config.quantumRR);}
    if ("LOTTERY" == algorithm_choice) {execution_timeline = lottery_scheduling(listeProcessus, tailleListe(listeProcessus->tete));}
}

/**
 * @brief Charge un CSV et traite les métadonnées associées.
 *
 * En plus de créer la liste de processus via le `csv_reader`, cette méthode
 * analyse la chaîne de caractères du chemin pour extraire le nom du fichier
 * (ex: transforme "C:/data/test.csv" en "test.csv").
 *
 * @param sourcepath Chemin du fichier à charger.
 */
void AlgoController::setCSV(char *sourcepath) {

    // Libération de l'ancienne liste si elle existe
    if (AlgoController::listeProcessus != nullptr) {
        destroyLTQ(AlgoController::listeProcessus, (void (*)(void *)) libMemProcessus);
        AlgoController::listeProcessus = nullptr;
    }

    if (! isValidCSVFile(sourcepath)) {
        std::cerr << "Erreur : fichier CSV invalide.\n";
        AlgoController::listeProcessus = nullptr;
        currentCSVPath = "";
        currentCSVName = "";
        return;
    }

    // Importation des nouvelles données
    AlgoController::listeProcessus = createListeProcessusFromCSV(sourcepath);

    // Validation
    if (!isListeProcessusValid(AlgoController::listeProcessus)) {
        std::cerr << "[AlgoController] Erreur : liste de processus invalide." << std::endl;
        destroyLTQ(AlgoController::listeProcessus, (void (*)(void *)) libMemProcessus);
        AlgoController::listeProcessus = nullptr;

        // reset aussi les infos fichier
        currentCSVPath = "";
        currentCSVName = "";
        return;
    }

    // Stockage et parsing du nom de fichier
    currentCSVPath = sourcepath;
    std::string pathStr(sourcepath);
    size_t pos = pathStr.find_last_of("/\\");
    if (pos != std::string::npos) {
        currentCSVName = pathStr.substr(pos + 1);
    } else {
        currentCSVName = pathStr;
    }
}

void AlgoController::setContentCSV(const std::string& csvContent) {

    if (listeProcessus != nullptr) {
        destroyLTQ(listeProcessus, (void (*)(void *)) libMemProcessus);
        listeProcessus = nullptr;
    }

    if (! isValidInlineCSV(csvContent)) {
        std::cerr << "[AlgoController] Erreur : String passe incorrect." << std::endl;
        listeProcessus = nullptr;
        currentCSVPath = "";
        currentCSVName = "";
        return;
    }

    listeProcessus = createListeProcessusFromString(csvContent);

    if (!isListeProcessusValid(listeProcessus)) {
        std::cerr << "[AlgoController] Erreur : liste de processus invalide." << std::endl;
        listeProcessus = nullptr;
        currentCSVPath = "";
        currentCSVName = "";
        return;
    }

    currentCSVPath = "";
    currentCSVName = "";
}


/**
 * Enregistre les choix de l'utilisateur
 *
 * @param algorithm Type d'algorithme souhaité.
 * @return ExecutionTimeline* Pointeur vers l'historique d'exécution généré.
 */
void AlgoController::selectAlgorithm(const char *algorithm, AlgoConfig config) {
    for (std::string_view name : ALGO){
        if (name == algorithm) {
            algorithm_choice = algorithm;
        }
    }
    if (config.quantumRR > 0)algorithm_config.quantumRR = config.quantumRR;
    else algorithm_config.quantumRR = 1;
}


bool AlgoController::canRunAlgorithm() {
    if (listeProcessus == nullptr || algorithm_choice.empty()) {
        return false;
    }
    return true;
}

/**
 * @brief Récupère le chemin complet du fichier source.
 */
std::string AlgoController::getCurrentCSVPath() {
    return currentCSVPath;
}


/**
 * @brief Récupère uniquement le nom du fichier CSV pour un affichage plus propre en UI.
 */
std::string AlgoController::getCurrentCSVName() {
    return currentCSVName;
}

std::string AlgoController::getCurrentAlgorithmName() {
    return algorithm_choice;
}

ExecutionTimeline* AlgoController::getExecutionTimeline() {
    return execution_timeline;
}