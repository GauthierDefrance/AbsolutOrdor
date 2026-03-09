#pragma once
#include "process/utilities/csv_reader.h"
#include "data/struct/resultat.h"


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
         *
         * @param sourcepath
         */
        void setCSV(const char *sourcepath);


        /**
         *
         * @param algorithm
         * @return
         */
        Resultat* selectAlgorithm(SchedulingAlgorithm algorithm);

    private:


};