#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <cstdlib>

extern "C" {
    #include "../C/Core/liste_tq.h"
    #include "../C/Core/liste_generique.h"
    #include "../C/Core/data.h"
    #include "../C/Core/processus.h"
    #include "../C/Core/resultat.h"
    #include "../C/Core/structure.h"
}

// Macro de logging pour suivre l'avancee sans caracteres speciaux
#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

static void print_test(const char* name, bool ok) {
    std::cout << (ok ? "[OK]   " : "[FAIL] ") << name << "\n";
}

// ---------------------------------------------------------
// 1. Tests d'allocation et initialisation profonde
// ---------------------------------------------------------
bool test_deep_alloc_init() {
    LOG_STEP("Verification initialisation Processus...");
    Processus *p = allocMemProcessus();
    initProcessus(p);
    if (p->time_arrival != 0 || p->nb_quantum != 0 || strlen(p->name) != 0) return false;

    LOG_STEP("Verification buildData (Varargs)...");
    // Test de buildData pour un processus (Name, Time, Quantum)
    Data *dProc = buildData(TYPE_PROCESSUS, "TestProc", 10, 3);
    if (!dProc || dProc->type != TYPE_PROCESSUS) return false;
    if (strcmp(processusName((dProc->content.processus)), "TestProc") != 0) return false;

    libMemData(dProc);
    libMemProcessus(&p);
    return true;
}

// ---------------------------------------------------------
// 2. Tests ListeGenerique : Insertions et Suppressions
// ---------------------------------------------------------
bool test_liste_generique_advanced() {
    ListeGenerique l = nullptr;

    LOG_STEP("Insertion Tete vs Queue...");
    Data *d1 = buildData(TYPE_RESULTAT, 100);
    Data *d2 = buildData(TYPE_RESULTAT, 200);

    inserTete(&l, d1); // [100]
    inserQueue(&l, d2); // [100, 200]

    if (donnee(*(l->data.content.resultat)) != 100) return false;
    if (donnee(*(l->suivant->data.content.resultat)) != 200) return false;

    LOG_STEP("Suppression de la queue (O(N))...");
    suppQueue(&l); // Reste [100]
    if (l->suivant != nullptr) return false;

    LOG_STEP("Suppression du dernier element...");
    suppTete(&l); // Vide
    if (l != nullptr) return false;

    libMemData(d1); libMemData(d2);
    return true;
}

// ---------------------------------------------------------
// 3. Tests ListeTQ (Tete-Queue) : Coherence des pointeurs
// ---------------------------------------------------------
bool test_liste_tq_robustness() {
    ListeTQ ltq = allocMemLTQ();
    initLTQ(ltq);

    LOG_STEP("Test FIFO (First-In First-Out)...");
    for(int i=1; i<=3; i++) {
        inserQueueLTQ(ltq, buildData(TYPE_RESULTAT, i));
    }

    if (donnee(*(ltq->tete->data.content.resultat)) != 1) return false;
    if (donnee(*(ltq->queue->data.content.resultat)) != 3) return false;

    LOG_STEP("Defilement partiel...");
    suppTeteLTQ(ltq); // [2, 3]
    if (donnee(*(ltq->tete->data.content.resultat)) != 2) return false;
    if (ltq->queue->suivant != nullptr) return false;

    LOG_STEP("Suppression Queue sur LTQ...");
    suppQueueLTQ(ltq); // [2]
    if (ltq->tete != ltq->queue) return false;
    if (donnee(*(ltq->tete->data.content.resultat)) != 2) return false;

    destroyLTQ(ltq);
    return true;
}

// ---------------------------------------------------------
// 4. Test de Stress / Melange de types
// ---------------------------------------------------------
bool test_mixed_types_stress() {
    ListeTQ ltq = allocMemLTQ();
    initLTQ(ltq);

    LOG_STEP("Insertion alternee Processus/Resultat...");
    inserQueueLTQ(ltq, buildData(TYPE_PROCESSUS, "P1", 0, 1));
    inserQueueLTQ(ltq, buildData(TYPE_RESULTAT, 42));
    inserQueueLTQ(ltq, buildData(TYPE_PROCESSUS, "P2", 5, 2));

    int count = 0;
    ListeGenerique curr = ltq->tete;
    while(curr) {
        count++;
        curr = curr->suivant;
    }

    if (count != 3) return false;

    Data *mid = &(ltq->tete->suivant->data);
    if (mid->type != TYPE_RESULTAT || donnee(*(mid->content.resultat)) != 42) return false;

    destroyLTQ(ltq);
    return true;
}

// ---------------------------------------------------------
// 5. Cas limites (NULL, Listes vides, Strings longues)
// ---------------------------------------------------------
bool test_edge_cases_extended() {
    LOG_STEP("Suppression sur pointeur NULL...");
    suppTeteLTQ(nullptr);

    LOG_STEP("Nom de processus tres long (Troncation)...");
    const char* longName = "CeciEstUnNomTresLongQuiDepasseLargementLaLimiteDeCinquanteCaracteres";
    Data *d = buildData(TYPE_PROCESSUS, longName, 0, 0);
    if (strlen(processusName((d->content.processus))) >= NBMAXCHAR) return false;

    LOG_STEP("Transition Liste 1 element -> 0 element...");
    ListeTQ ltq = allocMemLTQ();
    initLTQ(ltq);
    inserTeteLTQ(ltq, d);
    suppQueueLTQ(ltq);
    if (ltq->tete != nullptr || ltq->queue != nullptr) return false;

    destroyLTQ(ltq);
    return true;
}

// ---------------------------------------------------------
// 6. Test de Deep Copy et Independance Memoire
// ---------------------------------------------------------
bool test_deep_copy_integrity() {
    LOG_STEP("Creation d'une donnee originale...");
    Data *original = buildData(TYPE_PROCESSUS, "Original", 10, 2);

    LOG_STEP("Copie profonde (deepCopyData)...");
    Data *copie = deepCopyData(original);

    if (copie == nullptr || copie == original) return false;
    if (copie->content.processus == original->content.processus) return false;

    LOG_STEP("Modification de l'original pour verifier l'independance...");
    strncpy(original->content.processus->name, "Modified", NBMAXCHAR-1);

    if (strcmp(processusName((copie->content.processus)), "Original") != 0) return false;

    libMemData(original);
    libMemData(copie);
    return true;
}

// ---------------------------------------------------------
// 7. Test de Stress - Volume d'insertions
// ---------------------------------------------------------
bool test_heavy_load() {
    ListeTQ ltq = allocMemLTQ();
    initLTQ(ltq);
    const int count = 100;

    LOG_STEP("Insertion de 100 elements...");
    for(int i = 0; i < count; i++) {
        inserQueueLTQ(ltq, buildData(TYPE_RESULTAT, i));
    }

    LOG_STEP("Verification de l'integrite du chainage...");
    int detected = 0;
    ListeGenerique curr = ltq->tete;
    while(curr) {
        detected++;
        curr = curr->suivant;
    }

    if(detected != count) return false;

    LOG_STEP("Vidage complet...");
    destroyLTQ(ltq);
    return true;
}

// ---------------------------------------------------------
// Runner
// ---------------------------------------------------------
int main() {
    std::vector<std::pair<const char*, bool(*)()>> tests = {
        {"Alloc and Init (buildData, initialisation)", test_deep_alloc_init},
        {"ListeGenerique (Insert/Suppr Advanced)", test_liste_generique_advanced},
        {"ListeTQ (FIFO and Coherence Tete/Queue)", test_liste_tq_robustness},
        {"Mixed Types and Stress count", test_mixed_types_stress},
        {"Edge Cases (NULL, Truncation, Emptying)", test_edge_cases_extended},
        {"Deep Copy (Independance memoire)", test_deep_copy_integrity},
        {"Heavy Load (100+ insertions/suppressions)", test_heavy_load}
    };

    int passed = 0;
    std::cout << "=== DEMARRAGE DES TESTS CORE APPROFONDIS ===\n\n";

    for (auto& t : tests) {
        bool result = t.second();
        print_test(t.first, result);
        if (result) passed++;
        std::cout << "--------------------------------------------\n";
    }

    std::cout << "\nRESUME : " << passed << "/" << tests.size() << " tests reussis.\n";
    return (passed == (int)tests.size()) ? 0 : 1;
}