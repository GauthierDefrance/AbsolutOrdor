#include <iostream>
#include <vector>

bool testListe();
bool testListeTQ();
bool testProcessus();
bool testCSVReader();
bool testExecutionTimeline();
bool testAlgoControllerFIFO();
bool testAlgoControllerSJF();

static void print_test(const char* name, bool ok) {
    std::cout << (ok ? "[OK]   " : "[FAIL] ") << name << "\n";
}

int main() {

    std::vector<std::pair<const char*, bool(*)()>> tests = {
        //Liste des algos lancé
        {"Structure Liste Simple",         testListe},
        {"Structure ListeTQ",              testListeTQ},
        {"Structure Processus",            testProcessus},

        {"Lecture CSV",                    testCSVReader},
        {"ExecutionTimeline depuis CSV",   testExecutionTimeline},

        {"AlgoController FIFO",            testAlgoControllerFIFO},
        {"AlgoController SJF",            testAlgoControllerSJF}
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