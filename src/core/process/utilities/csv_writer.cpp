#include "csv_writer.h"
#include <fstream>
#include <iomanip>

void exportStatsCSV(const ExecutionTimeline *tl,const char* algo, AlgoConfig config, const std::string& outputPath, bool append) {

    bool fileExists = false;
    {
        std::ifstream check(outputPath);
        fileExists = check.good();
    }

    std::ofstream out(outputPath, append ? std::ios::app : std::ios::trunc);
    if (!out) {
        std::cerr << "Erreur : impossible d'ecrire dans " << outputPath << "\n";
        return;
    }

    if (!append || !fileExists) {
        out << "Algorithme;QuantumRR;NbProcessus;Debut;Fin;DureeTotale;"
               "TicksUC;TicksES;TicksWait;"
               "AttenteMoy;RestitutionMoy;ReponseMoy;TauxCPU\n";

    }

    double attenteMoy     = attenteMoyProcessus((ExecutionTimeline*)tl);
    double restitutionMoy = restitutionMoyProcessus((ExecutionTimeline*)tl);
    double reponseMoy     = tempRepMoyProcessus((ExecutionTimeline*)tl);
    double cpuUtil        = tauxOccupationCPU((ExecutionTimeline*)tl);
    int tDebut  = getTimelineDebut(tl);
    int tMax    = getTimelineMax(tl);
    int nbProc  = getTimelineNbProcessus(tl);
    int ticksUC = getTimelineTicksType(tl, UC);
    int ticksES = getTimelineTicksType(tl, ES);
    int ticksW  = getTimelineTicksType(tl, W);


    out << algo << ';'
        << config.quantumRR << ';'
        << nbProc << ';' << tDebut << ';' << tMax << ';' << (tMax - tDebut) << ';'
        << ticksUC << ';' << ticksES << ';' << ticksW << ';'
        << std::fixed << std::setprecision(2)
        << attenteMoy << ';' << restitutionMoy << ';' << reponseMoy << ';' << cpuUtil
        << '\n';

    out << "\n";
    out << "Algorithme;Processus;Arrivee;TicksUC;TicksES;TicksWait;Restitution;Reponse\n";


    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus *) cell->data;

        int pUC = 0, pES = 0, pW = 0;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            Quantum *q = (Quantum *) qc->data;
            if      (q->type == UC) pUC += q->nbQuantum;
            else if (q->type == ES) pES += q->nbQuantum;
            else                    pW  += q->nbQuantum;
        }

        int restitution = tempsRestitutionProcessus(p);
        int reponse     = tempsRepProcessus(p);

        out << algo << ';'
            << p->name << ';' << p->timeArrival << ';'
            << pUC << ';' << pES << ';' << pW << ';'
            << restitution << ';' << reponse << '\n';

    }

    out << "\n";
    std::cout << "Resultats exportes dans : " << outputPath << "\n";
}
