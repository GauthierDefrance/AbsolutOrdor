#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <fstream>
#include <iostream>

extern "C" {
        #include "data/struct/processus.h"
        #include "data/struct/liste_tq.h"
    }


/**
 * @brief Parse un flux CSV et construit une liste de processus.
 * @param in Flux d'entrée (fichier ou mémoire).
 * @return Liste de processus (peut être vide si flux invalide/vide).
 */
ListeTQ createListeProcessusFromStream(std::istream &in);

/**
 * @brief Parse une chaîne CSV (supporte les "\\n") et construit une liste de processus.
 * @param content Contenu CSV sous forme de string.
 * @return Liste de processus (peut être vide si invalide).
 */
ListeTQ createListeProcessusFromString(const std::string &content);

/**
 * @brief Vérifie qu'un CSV inline est valide (ignore lignes vides et commentaires '#').
 * @param input Contenu CSV (peut contenir "\\n").
 * @return true si le format et les nombres sont valides, sinon false.
 */
bool isValidInlineCSV(const std::string& input);

/**
 * @brief Vérifie qu'un fichier CSV est valide.
 * @param filepath Chemin du fichier.
 * @return true si le fichier est lisible et valide, sinon false.
 */
bool isValidCSVFile(const char* filepath);