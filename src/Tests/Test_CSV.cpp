#include <iostream>
#include <fstream>


#include "process/utilities/string_parser.h"

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testCSVReader() {

    const char* filename = "input_test.csv";


    std::ofstream f(filename, std::ios::trunc);
    f << "3\n";
    f << "P1;0;5;10\n";
    f << "P2;2;8\n";
    f << "P3;4;3;4;2\n";
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    ListeTQ liste = createListeProcessusFromStream(file);

    if(!liste)return false;

    destroyLTQ(liste,(void(*)(void*))libMemProcessus);

    remove(filename);

    return true;
}