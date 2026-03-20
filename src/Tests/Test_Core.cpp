#include <iostream>
#include <vector>
#include <ctime>   // Pour std::time
#include <cstdlib> // Pour std::srand

bool testListe();
bool testListeTQ();
bool testProcessus();
bool testCSVReader();
bool testExecutionTimeline();
bool testAlgoControllerFIFO();
bool testAlgoControllerSJF();
bool testAlgoControllerSJRF();
bool testAlgoControllerLotteryScheduling();

static void print_test(const char* name, bool ok) {
    std::cout << (ok ? "[OK]   " : "[FAIL] ") << name << "\n";
}

int main() {
    // --- INITIALISATION DE L'ALÉATOIRE ---
    std::srand(std::time(nullptr));

    std::vector<std::pair<const char*, bool(*)()>> tests = {
        //Liste des algos lancé
        {"Structure Liste Simple",         testListe},
        {"Structure ListeTQ",              testListeTQ},
        {"Structure Processus",            testProcessus},

        {"Lecture CSV",                    testCSVReader},
        {"ExecutionTimeline depuis CSV",   testExecutionTimeline},

        {"AlgoController FIFO",            testAlgoControllerFIFO},
        {"AlgoController SJF",             testAlgoControllerSJF},
        {"AlgoController SJRF",            testAlgoControllerSJRF},
        {"AlgoController LOTTERY",         testAlgoControllerLotteryScheduling}
    };

    int passed = 0;

    std::cout << "=== DEMARRAGE DES TESTS CORE ===\n";

    for (auto& t : tests) {

        std::cout << "[RUN]  " << t.first << "...\n";

        bool result = t.second();

        print_test(t.first, result);

        if (result)
            passed++;

        std::cout << "--------------------------------------------\n";
    }

    std::cout << "\nRESUME : " << passed << "/" << tests.size() << " tests reussis.\n";

    return (passed == (int) tests.size()) ? 0 : 1;
}