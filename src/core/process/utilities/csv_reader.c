/**
* @file csv_reader.c
 * @brief Implémentation du parseur de fichiers de configuration de processus.
 *
 * Le lecteur supporte les commentaires (lignes débutant par #) et les lignes vides.
 * Il s'attend à un format spécifique :
 * - Une ligne pour le nombre total de processus.
 * - Pour chaque processus : nom;date_arrivee;UC1;ES1;UC2;ES2...
 */

#include "csv_reader.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>


/* Prototypes des utilitaires statiques (privés au module) */
static bool isCommentLine(FILE *f);
static void skipLine(FILE *f);
static void skipAllIgnoredLines(FILE *f);


/**
 * @brief Charge et transforme les données CSV en structures Processus.
 *
 * La fonction utilise une machine à états simple basée sur l'index de lecture
 * pour alterner les types de quanta : les indices pairs sont systématiquement
 * considérés comme du calcul (UC) et les indices impairs comme de l'E/S (ES).
 * Elle assure également le nettoyage des lignes inutiles (commentaires/espaces)
 * entre chaque bloc de données.
 *
 * @param fileName Nom du fichier source.
 * @return ListeTQ La liste peuplée de processus, prête pour l'ordonnanceur.
 */
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
    int i = 0;
    bool flag = true;
    while (i < size && flag) {
        i++;
        skipAllIgnoredLines(f);

        Processus *p = allocMemProcessus();
        initProcessus(p);

        // Lecture nom + date
        if (fscanf(f, "%[^;];%d;", processName, &spawnDate) != 2) {
            fprintf(stderr, "Erreur CSV ligne %d\n", i);
            free(p);
            flag = false;
        }

        strncpy(p->name, processName, NBMAXCHAR - 1);
        p->timeArrival = spawnDate;

        // Lecture UC/ES
        int index = 0;
        bool flag2 = true;
        while (fscanf(f, "%d", &quantum) == 1 && flag2) {
            Quantum *q = malloc(sizeof(Quantum));
            q->nbQuantum = quantum;
            q->type = (index % 2 == 0 ? UC : ES);

            inserQueueLTQ(p->listeTQ, q);

            index++;

            int c = fgetc(f);
            if (c == '\n' || c == EOF)
                flag2 = false;
        }

        inserQueueLTQ(liste, p);
    }

    fclose(f);
    return liste;
}


/**
 * @brief Vérifie la validité du pointeur de liste.
 *
 * @param liste Liste à tester.
 * @return true si non NULL, false sinon.
 */
bool isListeProcessusValid(ListeTQ liste) {
    return (liste != NULL);
}


/***********************
 * Utilitaires CSV
 ***********************/


/**
 * @brief Détermine si la ligne actuelle doit être ignorée par le parseur.
 *
 * Analyse le premier caractère non blanc de la ligne. Si ce caractère est
 * un dièse (#) ou un saut de ligne, la ligne est considérée comme non informative.
 * Utilise ftell/fseek pour ne pas consommer de données inutilement.
 *
 * @param f Flux du fichier ouvert.
 * @return true si la ligne est un commentaire ou vide, false sinon.
 */
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


/**
 * @brief Consomme les caractères jusqu'à la fin de la ligne actuelle.
 *
 * @param f Flux du fichier.
 */
static void skipLine(FILE *f) {
    int c;
    while ((c = fgetc(f)) != EOF && c != '\n');
}


/**
 * @brief Nettoie le flux en sautant tous les blocs de commentaires successifs.
 *
 * Cette fonction est appelée avant chaque lecture importante (fscanf) pour
 * s'assurer que le curseur pointe sur une donnée valide et non sur du texte informatif.
 *
 * @param f Flux du fichier.
 */
static void skipAllIgnoredLines(FILE *f) {
    while (isCommentLine(f)) skipLine(f);
}

