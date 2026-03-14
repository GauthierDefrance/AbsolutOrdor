#include <stdio.h>
#include <stdlib.h>
#include "../src/core/process/utilities/csv_reader.h"
#include "../src/core/process/Algo/sjf.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <fichier.csv>\n", argv[0]);
        return 1;
    }

    // Lecture du CSV
    printf("Lecture du fichier : %s\n", argv[1]);
    ListeTQ liste = createListeProcessusFromCSV(argv[1]);
    if (!isListeProcessusValid(liste)) {
        fprintf(stderr, "Erreur : fichier CSV invalide\n");
        return 1;
    }

    // Calcul de la taille
    int taille = 0;
    Liste courant = teteLTQ(liste);
    while (courant != NULL) {
        taille++;
        courant = courant->suivant;
    }
    printf("Nombre de processus : %d\n", taille);

    // Lancement de SJF
    ExecutionTimeline *tl = sjf(liste, taille);
    if (!tl) {
        fprintf(stderr, "Erreur : échec de l'ordonnancement SJF\n");
        destroyLTQ(liste, (void(*)(void*)) libMemProcessus);
        return 1;
    }

    // Affichage
    //afficherTimeline(tl);

    // Libération mémoire
    destroyTimeline(tl);
    destroyLTQ(liste, (void(*)(void*)) libMemProcessus);
    printf("fin");

    return 0;
}