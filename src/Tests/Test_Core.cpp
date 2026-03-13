#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

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
 * Affichage console avec décalage selon timeArrival
 *
 * Exemple (LABEL_WIDTH=12, chaque tick = 1 char) :
 *
 *   t=         0    5    10   15
 *   P1       : UC UC UC ES ES ES W
 *   P2           W  W  UC UC
 * ========================= */
static void afficherTimelineAvecDecalage(ExecutionTimeline *tl) {
    if (!tl) return;

    const int LABEL_WIDTH = 12; // largeur du champ nom
    const int TICK_WIDTH  = 2;  // largeur par tick (1 char + 1 espace)

    // Calcul du temps max pour l'en-tête
    int tMax = 0;
    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus*)cell->data;
        int t = p->timeArrival;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant)
            t += ((Quantum*)qc->data)->nbQuantum;
        if (t > tMax) tMax = t;
    }

    // En-tête des temps
    std::cout << "\n";
    std::cout << std::string(LABEL_WIDTH + 2, ' ') << "t=";
    for (int t = 0; t <= tMax; t += 5) {
        // Position absolue du tick t
        int pos = t * TICK_WIDTH;
        // On a déjà écrit (t/5) groupes
        std::string marker = std::to_string(t);
        std::cout << marker;
        // Remplissage jusqu'au prochain multiple de 5
        int nextPos = (t + 5) * TICK_WIDTH;
        int printed = (int)marker.size();
        int spaces  = nextPos - pos - printed;
        if (spaces > 0) std::cout << std::string(spaces, ' ');
    }
    std::cout << "\n";

    // Séparateur
    std::cout << std::string(LABEL_WIDTH + 4 + tMax * TICK_WIDTH, '-') << "\n";

    // Chaque processus
    for (Liste cell = tl->processus->tete; cell; cell = cell->suivant) {
        Processus *p = (Processus*)cell->data;

        // Nom (tronqué/paddé)
        std::string nom(p->name);
        if ((int)nom.size() > LABEL_WIDTH) nom = nom.substr(0, LABEL_WIDTH);
        std::cout << nom << std::string(LABEL_WIDTH - (int)nom.size(), ' ') << ": ";

        // Décalage selon timeArrival
        std::cout << std::string(p->timeArrival * TICK_WIDTH, ' ');

        // Quantums
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            Quantum *q = (Quantum*)qc->data;
            char sym = (q->type == UC) ? 'C' : (q->type == ES) ? 'E' : 'W';
            for (int i = 0; i < q->nbQuantum; i++) {
                std::cout << sym;
                if (TICK_WIDTH > 1) std::cout << std::string(TICK_WIDTH - 1, ' ');
            }
        }
        std::cout << "\n";
    }

    std::cout << std::string(LABEL_WIDTH + 4 + tMax * TICK_WIDTH, '-') << "\n";
    std::cout << "  Legende : C=UC  E=ES  W=Wait\n\n";
}

/* =========================
 * Affichage brut du CSV chargé
 * ========================= */
static void afficherCSVCharge(ListeTQ liste) {
    if (!liste) return;

    std::cout << "\n===== PROCESSUS CHARGES (CSV) =====\n";
    std::cout << std::left
              << std::setw(14) << "Nom"
              << std::setw(10) << "Arrivee"
              << "Quantums\n";
    std::cout << std::string(50, '-') << "\n";

    for (Liste c = liste->tete; c; c = c->suivant) {
        Processus *p = (Processus*)c->data;
        std::cout << std::setw(14) << p->name
                  << std::setw(10) << p->timeArrival;
        for (Liste qc = p->listeTQ->tete; qc; qc = qc->suivant) {
            Quantum *q = (Quantum*)qc->data;
            std::cout << (q->type == UC ? "UC" : q->type == ES ? "ES" : "W")
                      << "(" << q->nbQuantum << ") ";
        }
        std::cout << "\n";
    }
    std::cout << std::string(50, '=') << "\n\n";
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
 * Test ExecutionTimeline depuis CSV
 * ========================= */
bool testExecutionTimeline() {
    LOG_STEP("Creation d'un CSV de test complet (20 processus)");

    const char* filename = "timeline_test.csv";
    FILE *f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "#ProcessName;DateApparition;Nb_UC1;Nb_ES1;...;Nb_UCN;Nb_ESN\n");
    fprintf(f, "20\n");
    fprintf(f, "# Jeu de test complet pour l'ordonnanceur\n");
    fprintf(f, "Process1;0;5;5;9;6;2;4\n");
    fprintf(f, "Process2;2;8;3;4\n");
    fprintf(f, "Process3;4;3;2\n");
    fprintf(f, "Process4;1;6;4;3;5\n");
    fprintf(f, "Process5;3;7;2\n");
    fprintf(f, "Process6;5;2;6;3\n");
    fprintf(f, "Process7;6;9;4;2;3\n");
    fprintf(f, "Process8;8;4;5\n");
    fprintf(f, "Process9;10;5;3;2\n");
    fprintf(f, "Process10;12;6;4\n");
    fprintf(f, "Process11;14;3;7;2\n");
    fprintf(f, "Process12;16;8;3;6\n");
    fprintf(f, "Process13;18;4;2\n");
    fprintf(f, "Process14;20;7;5;3\n");
    fprintf(f, "Process15;22;5;4\n");
    fprintf(f, "Process16;24;6;3;2\n");
    fprintf(f, "Process17;26;9;6;4;3\n");
    fprintf(f, "Process18;28;2;5\n");
    fprintf(f, "Process19;30;7;3;2\n");
    fprintf(f, "Process20;32;5;4;3\n");
    fclose(f);

    LOG_STEP("Lecture du CSV via createListeProcessusFromCSV");
    ListeTQ liste = createListeProcessusFromCSV((char*)filename);
    if (!liste) return false;
    if (!isListeProcessusValid(liste)) return false;

    LOG_STEP("Verification du nombre de processus (20)");
    int count = 0;
    for (Liste c = liste->tete; c != nullptr; c = c->suivant) count++;
    if (count != 20) {
        std::cerr << "  [ERR] Attendu 20 processus, obtenu " << count << "\n";
        destroyLTQ(liste, (void (*)(void*))libMemProcessus);
        remove(filename);
        return false;
    }

    LOG_STEP("Construction de la timeline depuis la liste de processus");
    ExecutionTimeline *tl = allocTimeline();
    if (!tl) {
        destroyLTQ(liste, (void (*)(void*))libMemProcessus);
        remove(filename);
        return false;
    }

    // On insère tous les processus lus dans la timeline
    for (Liste c = liste->tete; c != nullptr; c = c->suivant) {
        Processus *p = (Processus*)c->data;
        ajouterProcessusTimeline(tl, p);
    }

    LOG_STEP("Verification : la timeline contient bien 20 processus");
    int tlCount = 0;
    for (Liste c = tl->processus->tete; c != nullptr; c = c->suivant) tlCount++;
    if (tlCount != 20) {
        std::cerr << "  [ERR] Timeline : attendu 20, obtenu " << tlCount << "\n";
        destroyTimeline(tl);
        destroyLTQ(liste, (void (*)(void*))libMemProcessus);
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage complet de la timeline");
    afficherTimeline(tl);

    LOG_STEP("Verification du contenu de Process1 (UC(5)->ES(5)->UC(9)->ES(6)->UC(2)->ES(4))");
    Liste c = tl->processus->tete;
    Processus *p1 = (Processus*)c->data;
    if (strcmp(p1->name, "Process1") != 0) {
        std::cerr << "  [ERR] Premier processus attendu : Process1, obtenu : " << p1->name << "\n";
        destroyTimeline(tl);
        destroyLTQ(liste, (void (*)(void*))libMemProcessus);
        remove(filename);
        return false;
    }

    // Vérification séquence : UC(5), ES(5), UC(9), ES(6), UC(2), ES(4)
    struct { enum OperationProcessus type; int nb; } expected[] = {
        {UC, 5}, {ES, 5}, {UC, 9}, {ES, 6}, {UC, 2}, {ES, 4}
    };
    Liste qcell = p1->listeTQ->tete;
    for (int i = 0; i < 6; i++) {
        if (!qcell) {
            std::cerr << "  [ERR] Quantum manquant a l'index " << i << "\n";
            destroyTimeline(tl);
            destroyLTQ(liste, (void (*)(void*))libMemProcessus);
            remove(filename);
            return false;
        }
        Quantum *q = (Quantum*)qcell->data;
        if (q->type != expected[i].type || q->nbQuantum != expected[i].nb) {
            std::cerr << "  [ERR] Quantum[" << i << "] attendu : "
                      << (expected[i].type == UC ? "UC" : "ES") << "(" << expected[i].nb << ")"
                      << ", obtenu : "
                      << (q->type == UC ? "UC" : q->type == ES ? "ES" : "W")
                      << "(" << q->nbQuantum << ")\n";
            destroyTimeline(tl);
            destroyLTQ(liste, (void (*)(void*))libMemProcessus);
            remove(filename);
            return false;
        }
        qcell = qcell->suivant;
    }

    LOG_STEP("Destruction timeline et nettoyage");
    // destroyTimeline ne libere pas les Processus* — on les libere via la liste
    destroyTimeline(tl);
    destroyLTQ(liste, (void (*)(void*))libMemProcessus);
    remove(filename);

    return true;
}

/* =========================
 * Test AlgoController + SJF
 * ========================= */
// bool testAlgoControllerSJF() {
//     LOG_STEP("Creation d'un CSV temporaire pour AlgoController");
//
//     const char* filename = "algo_test.csv";
//     FILE* f = fopen(filename, "w");
//     if (!f) return false;
//
//     fprintf(f, "6\n");
//
//     fprintf(f, "P1;0;5;3;4\n");     // UC5 ES3 UC4
//     fprintf(f, "P2;1;2\n");         // UC2
//     fprintf(f, "P3;2;8\n");         // UC8
//     fprintf(f, "P4;4;1\n");         // UC1
//     fprintf(f, "P5;6;3;2;2\n");     // UC3 ES2 UC2
//     fprintf(f, "P6;12;4\n");        // UC4
//
//     fclose(f);
//
//     LOG_STEP("Chargement CSV via AlgoController");
//     AlgoController::setCSV((char*)filename);
//
//     LOG_STEP("Execution de l'algorithme SJF");
//     ExecutionTimeline* tl = AlgoController::selectAlgorithm(SJF);
//     if (!tl) {
//         remove(filename);
//         return false;
//     }
//
//     LOG_STEP("Affichage de la timeline SJF (verification visuelle)");
//     afficherTimeline(tl);
//
//     destroyTimeline(tl);
//     remove(filename);
//
//     return true;
// }



/* =========================
 * Test AlgoController + FIFO
 * ========================= */
bool testAlgoControllerFIFO() {
    LOG_STEP("Creation d'un CSV temporaire pour AlgoController FIFO");

    const char* filename = "fifo_test.csv";
    FILE* f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "# Test FIFO complet\n");
    fprintf(f, "8\n");

    fprintf(f, "P1;0;5;3;4\n");      // UC5 ES3 UC4
    fprintf(f, "P2;0;2\n");          // arrive en meme temps que P1
    fprintf(f, "P3;1;8\n");          // long job
    fprintf(f, "P4;2;1\n");          // job tres court
    fprintf(f, "P5;4;3;2;2\n");      // UC3 ES2 UC2
    fprintf(f, "P6;6;4\n");          // arrive pendant execution
    fprintf(f, "P7;10;2;2;3\n");     // UC2 ES2 UC3
    fprintf(f, "P8;35;5\n");         // arrive apres que tout soit fini (CPU idle test)

    fclose(f);

    LOG_STEP("Chargement CSV via AlgoController");
    AlgoController::setCSV((char*)filename);

    // Affichage du CSV chargé
    // On relit la liste via csv_reader pour l'affichage (AlgoController ne l'expose pas)
    ListeTQ listeAffichage = createListeProcessusFromCSV((char*)filename);
    if (listeAffichage) {
        afficherCSVCharge(listeAffichage);
        destroyLTQ(listeAffichage, (void (*)(void*))libMemProcessus);
    }

    LOG_STEP("Execution de l'algorithme FIFO");
    ExecutionTimeline* tl = AlgoController::selectAlgorithm(FIFO);
    if (!tl) {
        std::cerr << "  [ERR] FIFO a retourne nullptr\n";
        remove(filename);
        return false;
    }

    LOG_STEP("Affichage standard de la timeline");
    afficherTimeline(tl);

    LOG_STEP("Affichage avec decalage temporel");
    afficherTimelineAvecDecalage(tl);

    destroyTimeline(tl);
    remove(filename);
    return true;
}








/* =========================
 * Runner
 * ========================= */
int main() {
    std::vector<std::pair<const char*, bool(*)()>> tests = {
        {"Structure Liste Simple",         testListe},
        {"Structure ListeTQ",              testListeTQ},
        {"Structure Processus",            testProcessus},
        {"Lecture CSV",                    testCSVReader},
        {"ExecutionTimeline depuis CSV",   testExecutionTimeline},
        {"Test algo Controller + FIFO", testAlgoControllerFIFO}
        //{"AlgoController + SJF",         testAlgoControllerSJF}
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