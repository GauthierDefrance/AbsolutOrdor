#pragma once
#include <sstream>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <fstream>
#include <iostream>

extern "C" {
        #include "data/struct/processus.h"
        #include "data/struct/liste_tq.h"
    }

ListeTQ createListeProcessusFromStream(std::istream &in);
ListeTQ createListeProcessusFromString(const std::string &content);
bool isValidInlineCSV(const std::string& input);
bool isValidCSVFile(const char* filepath);