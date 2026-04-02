#include "csv_writer.h"

void exportStatsCSV(const ExecutionTimeline *tl,const char* algo, AlgoConfig config, const std::string& outputPath, bool append) {

    bool fileExists = false;
    {
        FILE* check = fopen(outputPath.c_str(), "r");
        if (check) { fileExists = true; fclose(check); }
    }

    FILE* out = fopen(outputPath.c_str(), append ? "a" : "w");
    if (!out) {
        std::cerr << "Erreur : impossible d'ecrire dans " << outputPath << "\n";
        return;
    }

    if (!append || !fileExists) {
        fprintf(out, "Algorithme;QuantumRR;NbProcessus;Debut;Fin;DureeTotale;"
                     "TicksUC;TicksES;TicksWait;"
                     "AttenteMoy;RestitutionMoy;ReponseMoy;TauxCPU\n");
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

    fprintf(out, "%s;%d;%d;%d;%d;%d;%d;%d;%d;%.2f;%.2f;%.2f;%.2f\n",
            algo,
            config.quantumRR,
            nbProc, tDebut, tMax, tMax - tDebut,
            ticksUC, ticksES, ticksW,
            attenteMoy, restitutionMoy, reponseMoy, cpuUtil);

    fprintf(out, "\n");
    fprintf(out, "Algorithme;Processus;Arrivee;TicksUC;TicksES;TicksWait;Restitution;Reponse\n");

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

        fprintf(out, "%s;%s;%d;%d;%d;%d;%d;%d\n",
                algo,
                p->name, p->timeArrival,
                pUC, pES, pW,
                restitution, reponse);
    }

    fprintf(out, "\n");
    fclose(out);
    std::cout << "Resultats exportes dans : " << outputPath << "\n";
}
