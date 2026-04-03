#include <iostream>
#include <cstdio>
#include <cstring>

#include "process/utilities/string_parser.h"

#define LOG_STEP(msg) std::cout << "  [STEP] " << msg << std::endl;

bool testCSVReader() {

    const char* filename = "input_test.csv";

    FILE *f = fopen(filename,"w");

    fprintf(f,"3\n");
    fprintf(f,"P1;0;5;10\n");
    fprintf(f,"P2;2;8\n");
    fprintf(f,"P3;4;3;4;2\n");

    fclose(f);
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