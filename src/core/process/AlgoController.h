#pragma once
#include <string>
#include <array>
#include "process/utilities/string_parser.h"

/**
 * @brief Inclusion des structures C dans un environnement C++.
 *
 * L'usage de extern "C" est indispensable pour empêcher le name mangling (décoration de nom)
 * et permettre au lieur de retrouver les fonctions compilées en C.
 */
extern "C" {
    #include "data/struct/liste_tq.h"
    #include "data/struct/liste.h"
    #include "data/struct/processus.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/Algo/fifo.h"
    #include "process/Algo/rrn.h"
    #include "process/Algo/sjf.h"
    #include "process/Algo/sjrf.h"
    #include "process/Algo/lottery_scheduling.h"
}


struct AlgoConfig {
    int quantumRR;
    // Autres params éventuels
};

/**
 * @class AlgoController
 * @brief Contrôleur central (Singleton) gérant le cycle de vie de la simulation.
 *
 * Cette classe fait le lien entre l'interface utilisateur (ou le main), le lecteur CSV
 * et les différents algorithmes d'ordonnancement. Elle stocke la liste des processus
 * chargée en mémoire et permet de lancer les simulations de manière centralisée.
 */
class AlgoController {
public:

    /**
     * @brief Accesseur pour l'instance unique du contrôleur (Pattern Singleton).
     *
     * @return AlgoController& Référence vers l'instance unique globale.
     */
    static AlgoController& getInstance();
    static constexpr std::array<std::string,5> ALGO = {"FIFO","SJF","SJRF","RR","LOTTERY"};
    static constexpr std::array<bool,5> ALGO_NEED_CONFIG_CHOICE = {false,false,false,true,false};

    /**
     * @brief Destructeur de classe.
     *
     * Assure la libération de la mémoire C (liste de processus) lors de la fermeture.
     */
    ~AlgoController();

    /**
     * @brief Charge un fichier CSV et initialise la liste des processus.
     *
     * Si une liste était déjà chargée, elle est automatiquement libérée avant
     * d'importer les nouvelles données.
     *
     * @param sourcepath Chemin complet vers le fichier CSV.
     */
    void setCSV(char *sourcepath);

    /**
     * @brief Charge Le contenu d'un fichier CSV et initialise la liste des processus.
     *
     * Si une liste était déjà chargée, elle est automatiquement libérée avant
     * d'importer les nouvelles données.
     *
     * @param sourcepath Chemin complet vers le fichier CSV.
     */
    void setContentCSV(const std::string& csvContent);

    /**
     * @brief Point d'entrée pour l'exécution d'un algorithme.
     *
     * @param algorithm L'algorithme choisi via l'énumération SchedulingAlgorithm.
     * @return ExecutionTimeline* Pointeur vers les résultats (timeline) ou nullptr en cas d'erreur.
     */
    static void selectAlgorithm(std::string algorithm, AlgoConfig config = {4});


    /**
     * @brief Lance l'algorithme sélectionné et produit une timeline d'exécution.
     *
     * Agit comme un dispatcher qui choisit l'algorithme à exécuter en fonction du choix utilisateur.
     */
    static void runAlgorithm();

    /** @return Vérifiez si tous les paramètres nécessaires ont été donnés et sont valides. */
    static bool canRunAlgorithm();

    /** @return Vérifier si la liste de processus est vide. */
    static bool listeProcessusIsEmpty();

    /** @return Si oui ou non l'algorithme a besoin de paramètres supplémentaires */
    static bool CurrentAlgorithmNeedConfigChoice();

    /** @return Le chemin complet du fichier CSV actuellement chargé. */
    static std::string getCurrentCSVPath();

    /** @return Le nom seul du fichier (sans le dossier) pour l'affichage. */
    static std::string getCurrentCSVName();

    /** @return Le nom de l'algorithme actuellement choisi */
    static std::string getCurrentAlgorithmName();

    /** @return La configuration actuelle pour l'algorithme */
    static AlgoConfig getCurrentAlgorithmConfig();
    /** @return Récupérer le résultat de l'exécution de l'algorithme  */
    static ExecutionTimeline *getExecutionTimeline();

private:

    /** @brief Liste générique stockant les Processus* issus du dernier CSV chargé. */
    static ListeTQ listeProcessus;

    /** @brief Mémoire interne du chemin du fichier. */
    static std::string currentCSVPath;

    /** @brief Mémoire interne du nom du fichier. */
    static std::string currentCSVName;
};
