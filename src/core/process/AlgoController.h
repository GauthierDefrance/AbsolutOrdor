#pragma once

extern "C" {
    #include "data/struct/liste_tq.h"
    #include "data/struct/liste.h"
    #include "data/struct/processus.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/utilities/csv_reader.h"
    #include "process/Algo/sjf.h"
    #include "data/struct/liste_tq.h"
    #include "process/utilities/csv_reader.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "Algo/sjf.h"
    #include "process/Algo/fifo.h"
}

enum SchedulingAlgorithm {
    FIFO,
    SJF,
    SJRF,
    RR
    // ADVICE : Ajouter vos autres algorithmes ici
};

class AlgoController {
public:

    /**
     * @return The singleton of the class
     */
    static AlgoController& getInstance();

    /**
     * Destructeur de classe
     */
    ~AlgoController();

    /**
     * Charge un CSV et génère la liste de Processus*
     */
    static void setCSV(char *sourcepath);

    /**
     * Sélectionne un algorithme et renvoie la timeline d’exécution
     */
    static ExecutionTimeline* selectAlgorithm(SchedulingAlgorithm algorithm);

private:

    /**
     * ListeTQ générique contenant des Processus*
     * Issue du CSV
     */
    static ListeTQ listeProcessus;
};
