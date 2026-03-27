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
std::array<std::string,5> a = {"FIFO","SJF","SJRF","RR","LOTTERY"};


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

void AlgoController::setCSV(const std::string& csvContent) {
    if (listeProcessus != nullptr) {
        destroyLTQ(listeProcessus, (void (*)(void *)) libMemProcessus);
        listeProcessus = nullptr;
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
 * @brief Dispatcher d'algorithme.
 *
 * Agit comme une fabrique (factory) qui choisit l'algorithme à exécuter en fonction
 * du choix utilisateur. Elle transmet la liste de processus actuelle aux fonctions C.
 *
 * @param algorithm Type d'algorithme souhaité.
 * @return ExecutionTimeline* Pointeur vers l'historique d'exécution généré.
 */
ExecutionTimeline* AlgoController::selectAlgorithm(const char* algorithm, AlgoConfig config) {
    if (AlgoController::listeProcessus == nullptr) {
        std::cerr << "[AlgoController] Aucun CSV chargé, impossible de lancer un algorithme." << std::endl;
        return nullptr;
    }

    if (strcmp("FIFO", algorithm)==0){return fifo(listeProcessus);}
    else if (strcmp("SJF", algorithm)==0) {return sjf(listeProcessus, tailleListe(listeProcessus->tete));}
    else if (strcmp("SJRF", algorithm)==0) {return sjrf(listeProcessus, tailleListe(listeProcessus->tete));}
    else if (strcmp("RR", algorithm)==0) {return rrn(listeProcessus, config.quantumRR);}
    else if (strcmp("LOTTERY", algorithm)==0) {return lottery_scheduling(listeProcessus, tailleListe(listeProcessus->tete));}
    else {
        std::cerr << "[AlgoController] Algorithme inconnu." << std::endl;
        return nullptr;
    }
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