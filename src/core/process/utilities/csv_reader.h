#pragma once
#include "data/struct/processus.h"
#include "data/struct/liste_tq.h"
#include <stdbool.h>


// DEPRECATED : On utilise String Parser désormais /!\


/**
 * @file csv_reader.h
 * @brief Interface pour le chargement de scénarios d'ordonnancement via fichiers CSV.
 */

/**
 * @brief Analyse un fichier CSV pour générer une liste de processus exploitable.
 *
 * Cette fonction parse un fichier structuré contenant le nombre de processus,
 * leurs noms, dates d'arrivée et leurs cycles de vie. Chaque processus est
 * reconstruit avec une alternance automatique de quanta de calcul (UC) et
 * d'entrées/sorties (ES).
 *
 * @param fileName Chemin vers le fichier .csv à charger.
 * @return ListeTQ Une liste de processus (Liste de pointeurs Processus*),
 * ou NULL en cas d'échec d'ouverture.
 */
ListeTQ createListeProcessusFromCSV(char *fileName);


/**
 * @brief Valide l'intégrité de la structure de données chargée.
 *
 * Vérifie si la liste de processus a été correctement allouée et n'est pas corrompue
 * avant d'entamer une simulation d'ordonnancement.
 *
 * @param liste La liste de processus à vérifier.
 * @return true Si la liste est utilisable.
 * @return false Si la liste est NULL ou invalide.
 */
bool isListeProcessusValid(ListeTQ liste);