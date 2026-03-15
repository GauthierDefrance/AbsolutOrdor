#include "csv_reader.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

static bool isCommentLine(FILE *f);
static void skipLine(FILE *f);
static void skipAllIgnoredLines(FILE *f);

ListeTQ createListeProcessusFromCSV(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (!f) {
        fprintf(stderr, "Erreur : impossible d’ouvrir %s\n", fileName);
        return NULL;
    }

    ListeTQ liste = allocMemLTQ();
    initLTQ(liste);

    int size = 0;
    skipAllIgnoredLines(f);
    fscanf(f, "%d", &size);

    char processName[NBMAXCHAR];
    int spawnDate;
    int quantum;

    for (int i = 0; i < size; i++) {
        skipAllIgnoredLines(f);

        Processus *p = allocMemProcessus();
        initProcessus(p);

        // Lecture nom + date
        if (fscanf(f, "%[^;];%d;", processName, &spawnDate) != 2) {
            fprintf(stderr, "Erreur CSV ligne %d\n", i);
            free(p);
            break;
        }

        strncpy(p->name, processName, NBMAXCHAR - 1);
        p->timeArrival = spawnDate;

        // Lecture UC/ES
        int index = 0;
        while (fscanf(f, "%d", &quantum) == 1) {
            Quantum *q = malloc(sizeof(Quantum));
            q->nbQuantum = quantum;
            q->type = (index % 2 == 0 ? UC : ES);

            inserQueueLTQ(p->listeTQ, q);

            index++;

            int c = fgetc(f);
            if (c == '\n' || c == EOF)
                break;
        }

        inserQueueLTQ(liste, p);
    }

    fclose(f);
    return liste;
}

bool isListeProcessusValid(ListeTQ liste) {
    return (liste != NULL);
}

/***********************
 * Utilitaires CSV
 ***********************/

static bool isCommentLine(FILE *f) {
    long pos = ftell(f);
    if (pos == -1) return false;

    int c;
    while ((c = fgetc(f)) != EOF && isspace(c) && c != '\n') {

    }

    bool result = (c == '#' || c == '\n' || c == EOF);

    fseek(f, pos, SEEK_SET);
    return result;
}

static void skipLine(FILE *f) {
    int c;
    while ((c = fgetc(f)) != EOF && c != '\n');
}

static void skipAllIgnoredLines(FILE *f) {
    while (isCommentLine(f)) skipLine(f);
}
