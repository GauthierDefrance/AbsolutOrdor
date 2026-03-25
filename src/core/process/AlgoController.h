#pragma once
#include <string>


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
    #include "process/utilities/csv_reader.h"
    #include "data/struct/liste_tq.h"
    #include "process/utilities/csv_reader.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/Algo/fifo.h"
    #include "process/Algo/rrn.h"
    #include "process/Algo/sjf.h"
    #include "process/Algo/sjrf.h"
    #include "process/Algo/lottery_scheduling.h"
}


/**
 * @enum SchedulingAlgorithm
 * @brief Énumération des algorithmes d'ordonnancement supportés par le contrôleur.
 */
enum SchedulingAlgorithm {
    FIFO,
    SJF,
    SJRF,
    RR,
    LOTTERY
    // ADVICE : Ajouter vos autres algorithmes ici
};


struct AlgoConfig {
    int quantumRR = 2;
    // futurs paramètres ici
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
     * @brief Point d'entrée pour l'exécution d'un algorithme.
     *
     * @param algorithm L'algorithme choisi via l'énumération SchedulingAlgorithm.
     * @return ExecutionTimeline* Pointeur vers les résultats (timeline) ou nullptr en cas d'erreur.
     */
    static ExecutionTimeline* selectAlgorithm(SchedulingAlgorithm algorithm, AlgoConfig config = {});

    /** @return Le chemin complet du fichier CSV actuellement chargé. */
    std::string getCurrentCSVPath();

    /** @return Le nom seul du fichier (sans le dossier) pour l'affichage. */
    std::string getCurrentCSVName();

private:

    /** @brief Liste générique stockant les Processus* issus du dernier CSV chargé. */
    static ListeTQ listeProcessus;

    /** @brief Mémoire interne du chemin du fichier. */
    static std::string currentCSVPath;

    /** @brief Mémoire interne du nom du fichier. */
    static std::string currentCSVName;
};
