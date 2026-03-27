/**
 * @file Test_Inline.cpp
 * @brief Test un flux inline pour verifier la creation des processus depuis string_parser
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>

extern "C" {
    #include "data/struct/liste.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/processus.h"
}

#include "process/utilities/string_parser.h"

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;
bool testInlineCSV() {
    LOG_STEP("Initialisation du contenu CSV inline");

    std::string csvContent = "2\nP1;0;3;2\nP2;1;4;1";

    LOG_STEP("Creation de la liste de processus depuis string_parser");
    ListeTQ liste = createListeProcessusFromString(csvContent);

    if (!liste || !liste->tete) {
        std::cerr << "Erreur : ListeTQ vide\n";
        return false;
    }

    LOG_STEP("Verification du nombre de processus et des donnees");

    int count = 0;
    for (Liste cell = liste->tete; cell; cell = cell->suivant) {
        Processus* p = (Processus*) cell->data;
        if (!p) return false;

        if (count == 0) {
            if (std::string(p->name) != "P1") return false;
            if (p->timeArrival != 0) return false;
        } else if (count == 1) {
            if (std::string(p->name) != "P2") return false;
            if (p->timeArrival != 1) return false;
        }
        count++;
    }

    if (count != 2) {
        std::cerr << "Erreur : nombre de processus incorrect : " << count << "\n";
        return false;
    }

    LOG_STEP("Liberation memoire");
    destroyLTQ(liste, (void(*)(void*))libMemProcessus);  // ← suffit, pas de libMemLTQ

    LOG_STEP("Test inline CSV reussi");
    return true;
}