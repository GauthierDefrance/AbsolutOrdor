#pragma once
#include "process/AlgoController.h"
extern "C" {
    #include "TimelineStatsCalculator.h"
    #include "data/struct/ExecutionTimeline.h"
}

/**
 * Génère un CSV avec les résultats et statitisques
 * @param tl La timeline source
 * @param algo pour quel algo
 * @param config les paramètres particuliers tel que "-n"
 * @param outputPath le nom du fichier dans lequel exporter le résultat
 * @param append ajouter à la fin du fichier ou non
 */
void exportStatsCSV(const ExecutionTimeline *tl,const char* algo, AlgoConfig config, const std::string& outputPath, bool append = false);
