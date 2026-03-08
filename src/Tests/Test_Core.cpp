#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <cstdlib>

extern "C" {
    #include "../C/Core/liste_tq.h"
    #include "../C/Core/liste.h"
    #include "../C/Core/processus.h"
    #include "../C/Core/resultat.h"
    #include "../C/Core/csv_reader.h"
}

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

static void print_test(const char* name, bool ok) {
    std::cout << (ok ? "[OK]   " : "[FAIL] ") << name << "\n";
}

bool testTabProcessusCSV() {
    LOG_STEP("Creation d'un fichier CSV temporaire");

    // Cree un CSV temporaire
    const char* filename = "input.csv";
    FILE* f = fopen(filename, "w");
    if (!f) return false;

    // Contenu avec commentaires et lignes vides
    fprintf(f, "# Ceci est un commentaire\n");
    fprintf(f, "\n");
    fprintf(f, "3\n");              // Nombre de processus
    fprintf(f, "# Commentaire\n");
    fprintf(f, "P1;0;5\n");
    fprintf(f, "P2;2;8\n");
    fprintf(f, "P3;4;3\n");
    fclose(f);

    LOG_STEP("Lecture du CSV via createTabProcessusFromCSV");
    TabProcessus tab = createTabProcessusFromCSV((char*)filename);
    if (!tab) return false;

    LOG_STEP("Verification du nombre de processus");
    if (tab->nbProcess != 3) return false;

    LOG_STEP("Verification des donnees de chaque processus");
    if (strcmp(tab->tabProcess[0].name, "P1") != 0) return false;
    if (tab->tabProcess[0].timeArrival != 0) return false;
    if (tab->tabProcess[0].nbQuantum != 5) return false;

    if (strcmp(tab->tabProcess[1].name, "P2") != 0) return false;
    if (tab->tabProcess[1].timeArrival != 2) return false;
    if (tab->tabProcess[1].nbQuantum != 8) return false;

    if (strcmp(tab->tabProcess[2].name, "P3") != 0) return false;
    if (tab->tabProcess[2].timeArrival != 4) return false;
    if (tab->tabProcess[2].nbQuantum != 3) return false;

    LOG_STEP("Liberation memoire");
    freeTabProcessus(&tab);
    if (tab != NULL) return false;

    // Supprime le fichier temporaire
    remove(filename);

    return true;
}

/**
 * Test de la Liste Simple (liste.c / liste.h)
 */
bool testListe() {
    LOG_STEP("Creation et insertion en tete");
    Liste l = NULL;
    inserTete(&l, 10);
    inserTete(&l, 20); // Liste: 20 -> 10

    if (donneeListe(l) != 20) return false;
    if (donneeListe(suivantListe(l)) != 10) return false;

    LOG_STEP("Test Insertion en queue");
    inserQueue(&l, 30); // Liste: 20 -> 10 -> 30
    if (donneeListe(queueListe(l)) != 30) return false;

    LOG_STEP("Suppression");
    suppTete(&l); // Reste 10 -> 30
    if (donneeListe(l) != 10) return false;

    destroyListe(l);
    return true;
}

/**
 * Test de la Liste Tête-Queue (liste_tq.c / liste_tq.h)
 */
bool testListeTQ() {
    LOG_STEP("Allocation et Initialisation LTQ");
    ListeTQ ltq = allocMemLTQ();
    if (!ltq) return false;
    initLTQ(ltq);

    LOG_STEP("Insertion Queue (O(1))");
    inserQueueLTQ(ltq, 100);
    inserQueueLTQ(ltq, 200);

    if (donneeListe(teteLTQ(ltq)) != 100) return false;
    if (donneeListe(queueLTQ(ltq)) != 200) return false;

    LOG_STEP("Suppression Queue");
    suppQueueLTQ(ltq);
    if (donneeListe(queueLTQ(ltq)) != 100) return false;

    destroyLTQ(ltq);
    return true;
}

/**
 * Test des structures Processus (processus.c / processus.h)
 */
bool testProcessus() {
    LOG_STEP("Allocation Processus");
    Processus* p = allocMemProcessus();
    if (!p) return false;
    initProcessus(p);

    LOG_STEP("Modification des donnees");
    // On utilise un cast car processusName retourne const char*
    strncpy(p->name, "TestProc", NBMAXCHAR - 1);
    p->timeArrival = 5;
    p->nbQuantum = 10;

    if (strcmp(processusName(p), "TestProc") != 0) return false;
    if (timeArrival(*p) != 5) return false;
    if (nbQuantum(*p) != 10) return false;

    libMemProcessus(&p);
    return (p == NULL);
}

/**
 * Test de la structure Resultat (resultat.c / resultat.h)
 */
bool testResultat() {
    int nbP = 3;
    LOG_STEP("Allocation Resultat");
    Resultat* res = allocMemResultat(nbP);
    if (!res) return false;

    if (nbProcess(res) != 3) return false;

    LOG_STEP("Verification du tableau de processus");
    Processus* tab = getProcess(res);
    if (!tab) return false;

    // Test d'acces au premier element du tableau calloc
    if (tab[0].timeArrival != 0) return false;

    libMemResultat(res);
    return true;
}

// ---------------------------------------------------------
// Runner
// ---------------------------------------------------------
int main() {
    std::vector<std::pair<const char*, bool(*)()>> tests = {
        {"Structure Liste Simple", testListe},
        {"Structure ListeTQ",      testListeTQ},
        {"Structure Processus",    testProcessus},
        {"Structure Resultat",     testResultat},
        {"Test lecture CSV", testTabProcessusCSV}
    };

    int passed = 0;
    std::cout << "=== DEMARRAGE DES TESTS CORE REFACTORISES ===\n";

    for (auto& t : tests) {
        std::cout << "[RUN]  " << t.first << "..." << std::endl;
        bool result = t.second();
        print_test(t.first, result);
        if (result) passed++;
        std::cout << "--------------------------------------------\n";
    }

    std::cout << "\nRESUME : " << passed << "/" << tests.size() << " tests reussis.\n";

    if (passed < (int)tests.size()) {
        std::cout << "ATTENTION : Certains tests ont echoue !\n";
    }

    return (passed == (int)tests.size()) ? 0 : 1;
}