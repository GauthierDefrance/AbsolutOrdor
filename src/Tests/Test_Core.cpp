#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
    #include "data/struct/liste.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/processus.h"
    #include "data/struct/ExecutionTimeline.h"
    #include "process/utilities/csv_reader.h"
}

#include "process/AlgoController.h"

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

static void print_test(const char* name, bool ok) {
    std::cout << (ok ? "[OK]   " : "[FAIL] ") << name << "\n";
}

/* =========================
 * Test de la liste simple
 * ========================= */
bool testListe() {
    LOG_STEP("Creation liste simple et insertion de 3 entiers");

    Liste l = nullptr;

    int *a = (int*)malloc(sizeof(int));
    int *b = (int*)malloc(sizeof(int));
    int *c = (int*)malloc(sizeof(int));
    *a = 10; *b = 20; *c = 30;

    inserTete(&l, a); // [10]
    inserTete(&l, b); // [20] -> [10]
    inserQueue(&l, c); // [20] -> [10] -> [30]

    if (!l) return false;
    if (*(int*)donneeListe(l) != 20) return false;
    if (*(int*)donneeListe(suivantListe(l)) != 10) return false;
    if (*(int*)donneeListe(queueListe(l)) != 30) return false;

    LOG_STEP("Suppression de la tete");
    suppTete(&l, free); // libere 20, reste [10] -> [30]

    if (!l) return false;
    if (*(int*)donneeListe(l) != 10) return false;

    LOG_STEP("Destruction de la liste");
    destroyListe(l, free);

    return true;
}

/* =========================
 * Test de la liste Tete-Queue
 * ========================= */
bool testListeTQ() {
    LOG_STEP("Allocation et initialisation ListeTQ");
    ListeTQ ltq = allocMemLTQ();
    if (!ltq) return false;
    initLTQ(ltq);

    LOG_STEP("Insertion en queue de 2 entiers");
    int *x = (int*)malloc(sizeof(int));
    int *y = (int*)malloc(sizeof(int));
    *x = 100; *y = 200;

    inserQueueLTQ(ltq, x);
    inserQueueLTQ(ltq, y);

    if (!teteLTQ(ltq) || !queueLTQ(ltq)) return false;
    if (*(int*)donneeListe(teteLTQ(ltq)) != 100) return false;
    if (*(int*)donneeListe(queueLTQ(ltq)) != 200) return false;

    LOG_STEP("Suppression de la queue");
    suppQueueLTQ(ltq, free); // libere 200, reste 100

    if (!queueLTQ(ltq)) return false;
    if (*(int*)donneeListe(queueLTQ(ltq)) != 100) return false;

    LOG_STEP("Destruction ListeTQ");
    destroyLTQ(ltq, free);

    return true;
}

/* =========================
 * Test de Processus
 * ========================= */
bool testProcessus() {
    LOG_STEP("Allocation et initialisation d'un Processus");
    Processus *p = allocMemProcessus();
    if (!p) return false;
    initProcessus(p);

    strncpy(p->name, "TestProc", NBMAXCHAR - 1);
    p->timeArrival = 5;

    LOG_STEP("Ajout de 2 Quantum (UC(3), ES(7))");
    Quantum *q1 = (Quantum*)malloc(sizeof(Quantum));
    Quantum *q2 = (Quantum*)malloc(sizeof(Quantum));
    q1->type = UC; q1->nbQuantum = 3;
    q2->type = ES; q2->nbQuantum = 7;

    inserQueueLTQ(p->listeTQ, q1);
    inserQueueLTQ(p->listeTQ, q2);

    if (strcmp(processusName(p), "TestProc") != 0) return false;
    if (getTimeArrival(p) != 5) return false;

    Liste lq = p->listeTQ->tete;
    if (!lq) return false;
    Quantum *qa = (Quantum*)lq->data;
    if (qa->type != UC || qa->nbQuantum != 3) return false;

    lq = lq->suivant;
    if (!lq) return false;
    Quantum *qb = (Quantum*)lq->data;
    if (qb->type != ES || qb->nbQuantum != 7) return false;

    LOG_STEP("Liberation du processus");
    libMemProcessus(p);
    p = nullptr;

    return true;
}

/* =========================
 * Test du CSV reader
 * ========================= */
bool testCSVReader() {
    LOG_STEP("Creation d'un CSV temporaire");

    const char* filename = "input_test.csv";
    FILE *f = fopen(filename, "w");
    if (!f) return false;

    // Format : nbProcess, puis lignes : name;arrival;quantum1;quantum2;...
    fprintf(f, "# Commentaire\n");
    fprintf(f, "3\n");
    fprintf(f, "P1;0;5;10\n");   // UC(5), ES(10)
    fprintf(f, "P2;2;8\n");      // UC(8)
    fprintf(f, "P3;4;3;4;2\n");  // UC(3), ES(4), UC(2)
    fclose(f);

    LOG_STEP("Lecture via createListeProcessusFromCSV");
    ListeTQ liste = createListeProcessusFromCSV((char*)filename);
    if (!liste) return false;

    if (!isListeProcessusValid(liste)) return false;

    LOG_STEP("Verification du nombre de processus (3)");
    int count = 0;
    for (Liste c = liste->tete; c != nullptr; c = c->suivant) count++;
    if (count != 3) return false;

    LOG_STEP("Verification des donnees du premier processus");
    Liste c = liste->tete;
    Processus *p1 = (Processus*)c->data;
    if (strcmp(p1->name, "P1") != 0) return false;
    if (p1->timeArrival != 0) return false;

    // Quantum : UC(5), ES(10)
    Liste q = p1->listeTQ->tete;
    if (!q) return false;
    Quantum *q1 = (Quantum*)q->data;
    if (q1->type != UC || q1->nbQuantum != 5) return false;

    q = q->suivant;
    if (!q) return false;
    Quantum *q2 = (Quantum*)q->data;
    if (q2->type != ES || q2->nbQuantum != 10) return false;

    LOG_STEP("Nettoyage : destruction liste + fichier");
    destroyLTQ(liste, (void (*)(void*))libMemProcessus);
    liste = nullptr;
    remove(filename);

    return true;
}

/* =========================
 * Test ExecutionTimeline
 * ========================= */
bool testExecutionTimeline() {
    LOG_STEP("Creation de 2 processus et d'une timeline");

    Processus *p1 = allocMemProcessus();
    Processus *p2 = allocMemProcessus();
    if (!p1 || !p2) return false;
    initProcessus(p1);
    initProcessus(p2);

    strncpy(p1->name, "P1", NBMAXCHAR - 1);
    strncpy(p2->name, "P2", NBMAXCHAR - 1);

    Quantum *q1 = (Quantum*)malloc(sizeof(Quantum));
    Quantum *q2 = (Quantum*)malloc(sizeof(Quantum));
    q1->type = UC; q1->nbQuantum = 3;
    q2->type = ES; q2->nbQuantum = 5;

    inserQueueLTQ(p1->listeTQ, q1);
    inserQueueLTQ(p2->listeTQ, q2);

    ExecutionTimeline *tl = allocTimeline();
    if (!tl) return false;

    ajouterProcessusTimeline(tl, p1);
    ajouterProcessusTimeline(tl, p2);

    LOG_STEP("Verification de la timeline");
    if (!tl->processus || !tl->processus->tete) return false;

    Liste c = tl->processus->tete;
    Processus *tp1 = (Processus*)c->data;
    if (strcmp(tp1->name, "P1") != 0) return false;

    c = c->suivant;
    if (!c) return false;
    Processus *tp2 = (Processus*)c->data;
    if (strcmp(tp2->name, "P2") != 0) return false;

    LOG_STEP("Affichage (visuel) de la timeline");
    afficherTimeline(tl);

    LOG_STEP("Destruction timeline (ne libere pas les Processus)");
    destroyTimeline(tl);

    LOG_STEP("Liberation des processus");
    libMemProcessus(p1);
    libMemProcessus(p2);
    p1 = nullptr;
    p2 = nullptr;
    return true;
}

/* =========================
 * Test AlgoController + SJF
 * ========================= */
bool testAlgoControllerSJF() {
    LOG_STEP("Creation d'un CSV temporaire pour AlgoController");

    const char* filename = "algo_test.csv";
    FILE* f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "3\n");
    fprintf(f, "P1;0;5\n");
    fprintf(f, "P2;2;8\n");
    fprintf(f, "P3;4;3\n");
    fclose(f);

    LOG_STEP("Chargement CSV via AlgoController");
    AlgoController::setCSV((char*)filename);

    LOG_STEP("Execution de l'algorithme SJF");
    ExecutionTimeline* tl = AlgoController::selectAlgorithm(SJF);
    if (!tl) {
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage de la timeline SJF (verification visuelle)");
    afficherTimeline(tl);

    destroyTimeline(tl);
    remove(filename);

    return true;
}

/* =========================
 * Runner
 * ========================= */
int main() {
    std::vector<std::pair<const char*, bool(*)()>> tests = {
        {"Structure Liste Simple",   testListe},
        {"Structure ListeTQ",        testListeTQ},
        {"Structure Processus",      testProcessus},
        {"Lecture CSV",              testCSVReader},
        {"ExecutionTimeline",        testExecutionTimeline}
        //{"AlgoController + SJF",     testAlgoControllerSJF}
    };

    int passed = 0;
    std::cout << "=== DEMARRAGE DES TESTS CORE ===\n";

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
