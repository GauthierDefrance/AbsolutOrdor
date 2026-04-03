/**
 * @file string_parser.cpp
 * @brief Parseur de processus pour AbsolutOrdor basé sur flux mémoire.
 *
 * Supporte :
 * - Commentaires (#)
 * - Lignes vides
 * - Quanta alternés UC/ES automatiquement
 */

#include "string_parser.h"


/**
 * @brief Parse un flux mémoire pour créer une liste de processus.
 *
 * Fonctionne aussi bien pour un fichier via std::ifstream que pour un stringstream.
 *
 * @param in Flux mémoire contenant le CSV
 * @return ListeTQ Liste de processus, vide si erreur ou flux vide.
 */
ListeTQ createListeProcessusFromStream(std::istream &in) {
    ListeTQ liste = allocMemLTQ();
    initLTQ(liste);

    std::string line;

    // Ignorer les lignes vides ou commentaires
    auto readNextLine = [&]() -> bool {
        while (std::getline(in, line)) {
            // Supprime les espaces de début
            size_t start = line.find_first_not_of(" \t\r\n");
            if (start == std::string::npos || line[start] == '#') continue;
            line = line.substr(start);
            return true;
        }
        return false;
    };

    if (!readNextLine()) return liste; // flux vide

    // Nombre de processus
    int nbProcessus = std::stoi(line);

    for (int i = 0; i < nbProcessus; ++i) {
        if (!readNextLine()) break;

        Processus* p = allocMemProcessus();
        initProcessus(p);

        std::istringstream iss(line);
        std::string token;

        // Nom
        if (!std::getline(iss, token, ';')) { libMemProcessus(p); continue; }
        strncpy(p->name, token.c_str(), NBMAXCHAR - 1);
        p->name[NBMAXCHAR - 1] = '\0';

        // Date d'arrivée
        if (!std::getline(iss, token, ';')) { libMemProcessus(p); continue; }
        p->timeArrival = std::stoi(token);

        // Quanta UC/ES alternés
        int index = 0;
        while (std::getline(iss, token, ';')) {
            int nbTicks = std::stoi(token);
            Quantum* q = allocQuantum((index % 2 == 0 ? UC : ES), nbTicks);
            inserQueueLTQ(p->listeTQ, q);
            ++index;
        }

        inserQueueLTQ(liste, p);
    }

    return liste;
}

ListeTQ createListeProcessusFromString(const std::string &content) {
    std::string converted;
    for (size_t i = 0; i < content.size(); i++) {
        if (content[i] == '\\' && i + 1 < content.size() && content[i+1] == 'n') {
            converted += '\n';
            i++;
        } else {
            converted += content[i];
        }
    }

    std::istringstream iss(converted);
    ListeTQ liste = createListeProcessusFromStream(iss);
    if (!liste) {
        std::cerr << "Erreur : liste de processus NULL\n";
    }
    return liste;
}




bool isValidInlineCSV(const std::string& input) {
    std::string converted;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\' && i + 1 < input.size() && input[i+1] == 'n') {
            converted += '\n';
            i++;
        } else {
            converted += input[i];
        }
    }

    std::istringstream in(converted);
    std::string line;

    auto readNextLine = [&]() -> bool {
        while (std::getline(in, line)) {
            size_t start = line.find_first_not_of(" \t\r\n");
            if (start == std::string::npos || line[start] == '#') continue;
            line = line.substr(start);
            return true;
        }
        return false;
    };

    if (!readNextLine()) return false;
    int nbProcessus = 0;
    try { nbProcessus = std::stoi(line); }
    catch (...) { return false; }
    if (nbProcessus <= 0) return false;

    int found = 0;
    while (readNextLine() && found < nbProcessus) {
        std::istringstream iss(line);
        std::string token;

        if (!std::getline(iss, token, ';') || token.empty()) return false;

        if (!std::getline(iss, token, ';')) return false;
        int arrival = 0;
        try { arrival = std::stoi(token); }
        catch (...) { return false; }
        if (arrival < 0) return false;

        if (!std::getline(iss, token, ';')) return false;
        try { if (std::stoi(token) <= 0) return false; }
        catch (...) { return false; }

        while (std::getline(iss, token, ';')) {
            try { if (std::stoi(token) <= 0) return false; }
            catch (...) { return false; }
        }

        ++found;
    }

    return found >= nbProcessus;
}


bool isValidCSVFile(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir " << filepath << "\n";
        return false;
    }

    std::ostringstream oss;
    oss << file.rdbuf();

    return isValidInlineCSV(oss.str());
}



