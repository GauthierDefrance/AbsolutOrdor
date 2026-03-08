#include "csv_reader.h"
#include <string.h>
#include <ctype.h>

//***********************
// Allocation mémoire
//***********************

/**
 *
 * @param size
 * @return
 */
TabProcessus allocTabProcessus(int size) {
    if (size <= 0) return NULL;

    // Alloue la structure
    TabProcessus tab = (TabProcessus) malloc(sizeof(*tab));
    if (!tab) return NULL;

    tab->nbProcess = size;

    // Alloue le tableau de Processus
    tab->tabProcess = (Processus*) calloc(size, sizeof(Processus));
    if (!tab->tabProcess) {
        free(tab); // libère la structure si tableau non alloué
        return NULL;
    }

    return tab;
}




//***********************
// Libération mémoire
//***********************

/**
 *
 * @param tab
 */
void freeTabProcessus(TabProcessus *tab) {
    if (tab && *tab) {
        free((*tab)->tabProcess); // libère le tableau
        free(*tab);               // libère la structure
        *tab = NULL;              // évite dangling pointer
    }
}





//***********************
// Lecture du CSV et génération du tableau
//***********************

/**
 *
 * @param fileName
 * @return
 */
TabProcessus createTabProcessusFromCSV(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if (f==NULL) return NULL; // l'ouverture échoue

    // Lecture du nombre de données à lire.
    int size;
    skipAllIgnoredLines(f);
    fscanf(f, "%d", &size);
    TabProcessus tab = allocTabProcessus(size);
    if (tab==NULL) return NULL; //Echec de l'allocation mémoire

    // Lectures des données size fois
    int nbQuantums, spawnDate;
    char processName[NBMAXCHAR];
    for (int i=0; i<size; i++) {
        skipAllIgnoredLines(f);
        fscanf(f, "%[^;];%d;%d", //Le regex indique qu'on lit tous les characters possible sauf ;
            processName,
            &spawnDate,
            &nbQuantums);

        // Attribution des données
        strcpy(tab->tabProcess[i].name, processName);
        tab->tabProcess[i].nbQuantum = nbQuantums;
        tab->tabProcess[i].timeArrival = spawnDate;
    }

    fclose(f);
    return tab;
}





//***********************
// Fonctions utilitaires
//***********************

/**
 *
 * @param f
 * @return
 */

bool isCommentLine(FILE *f) {

    long pos = ftell(f);
    if (pos == -1) return false;

    int c;
    while ((c = fgetc(f)) != EOF && isspace(c) && c != '\n');

    bool result = (c == '#' || c == '\n' || c == EOF);

    fseek(f, pos, SEEK_SET);
    return result;
}

/**
 * Procédure permettant de skip une ligne commençant par #
 * @param f
 */
void skipLine(FILE *f) {
    if (f==NULL) {return;}
    int c;
    while ((c =fgetc(f)) != EOF && c != '\n');
}


/**
 * Procédure permettant de skip tous les commentaires jusqu'à la prochaine donnée utile
 * @param f
 */
void skipAllIgnoredLines(FILE *f) {
    if (f==NULL) {return;}
    while (isCommentLine(f)) skipLine(f);
}

bool isTabProcessusValid(TabProcessus tab) {
    if (tab == NULL) return false;
    if (tab->tabProcess == NULL) return false;
    if (tab->nbProcess <= 0) return false;

    return true;
}