#include "processus.h"
#include "resultat.h"
#include "../Algo/sjf.h"

int main() {
    int taille = 20;
    Processus tab[taille];

    initProcessus(&tab[0]);  strncpy(tab[0].name, "Process1",  NBMAXCHAR-1); tab[0].timeArrival = 0;  tab[0].nbQuantum = 5;
    initProcessus(&tab[1]);  strncpy(tab[1].name, "Process2",  NBMAXCHAR-1); tab[1].timeArrival = 2;  tab[1].nbQuantum = 8;
    initProcessus(&tab[2]);  strncpy(tab[2].name, "Process3",  NBMAXCHAR-1); tab[2].timeArrival = 4;  tab[2].nbQuantum = 3;
    initProcessus(&tab[3]);  strncpy(tab[3].name, "Process4",  NBMAXCHAR-1); tab[3].timeArrival = 1;  tab[3].nbQuantum = 6;
    initProcessus(&tab[4]);  strncpy(tab[4].name, "Process5",  NBMAXCHAR-1); tab[4].timeArrival = 3;  tab[4].nbQuantum = 7;
    initProcessus(&tab[5]);  strncpy(tab[5].name, "Process6",  NBMAXCHAR-1); tab[5].timeArrival = 5;  tab[5].nbQuantum = 2;
    initProcessus(&tab[6]);  strncpy(tab[6].name, "Process7",  NBMAXCHAR-1); tab[6].timeArrival = 6;  tab[6].nbQuantum = 9;
    initProcessus(&tab[7]);  strncpy(tab[7].name, "Process8",  NBMAXCHAR-1); tab[7].timeArrival = 8;  tab[7].nbQuantum = 4;
    initProcessus(&tab[8]);  strncpy(tab[8].name, "Process9",  NBMAXCHAR-1); tab[8].timeArrival = 10; tab[8].nbQuantum = 5;
    initProcessus(&tab[9]);  strncpy(tab[9].name, "Process10", NBMAXCHAR-1); tab[9].timeArrival = 12; tab[9].nbQuantum = 6;
    initProcessus(&tab[10]); strncpy(tab[10].name, "Process11", NBMAXCHAR-1); tab[10].timeArrival = 14; tab[10].nbQuantum = 3;
    initProcessus(&tab[11]); strncpy(tab[11].name, "Process12", NBMAXCHAR-1); tab[11].timeArrival = 16; tab[11].nbQuantum = 8;
    initProcessus(&tab[12]); strncpy(tab[12].name, "Process13", NBMAXCHAR-1); tab[12].timeArrival = 18; tab[12].nbQuantum = 4;
    initProcessus(&tab[13]); strncpy(tab[13].name, "Process14", NBMAXCHAR-1); tab[13].timeArrival = 20; tab[13].nbQuantum = 7;
    initProcessus(&tab[14]); strncpy(tab[14].name, "Process15", NBMAXCHAR-1); tab[14].timeArrival = 22; tab[14].nbQuantum = 5;
    initProcessus(&tab[15]); strncpy(tab[15].name, "Process16", NBMAXCHAR-1); tab[15].timeArrival = 24; tab[15].nbQuantum = 6;
    initProcessus(&tab[16]); strncpy(tab[16].name, "Process17", NBMAXCHAR-1); tab[16].timeArrival = 26; tab[16].nbQuantum = 9;
    initProcessus(&tab[17]); strncpy(tab[17].name, "Process18", NBMAXCHAR-1); tab[17].timeArrival = 28; tab[17].nbQuantum = 2;
    initProcessus(&tab[18]); strncpy(tab[18].name, "Process19", NBMAXCHAR-1); tab[18].timeArrival = 30; tab[18].nbQuantum = 7;
    initProcessus(&tab[19]); strncpy(tab[19].name, "Process20", NBMAXCHAR-1); tab[19].timeArrival = 32; tab[19].nbQuantum = 5;

    Resultat *resultat = sjf(tab, taille);
    if (resultat == NULL) {
        fprintf(stderr, "Erreur : sjf a retourné NULL\n");
        return 1;
    }

    afficherResultat(resultat);
    libMemResultat(resultat);

    return 0;
}