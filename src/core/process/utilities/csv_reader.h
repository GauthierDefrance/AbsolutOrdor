#pragma once

#include "data/struct/processus.h"
#include "data/struct/liste_tq.h"
#include <stdbool.h>

/**
 * @brief Lit un fichier CSV et retourne une ListeTQ de Processus*.
 * Chaque Processus contient une listeTQ de Quantum* (UC / ES uniquement).
 */
ListeTQ createListeProcessusFromCSV(char *fileName);

/**
 * @brief Vérifie si la liste est valide.
 */
bool isListeProcessusValid(ListeTQ liste);
