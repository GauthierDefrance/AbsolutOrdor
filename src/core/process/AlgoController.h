#pragma once
#include "../C/Core/csv_reader.h"
#include "../C/Core/resultat.h"


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