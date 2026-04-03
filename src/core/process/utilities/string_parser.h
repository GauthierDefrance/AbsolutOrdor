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


/**
 *
 * @param in
 * @return
 */
ListeTQ createListeProcessusFromStream(std::istream &in);


/**
 *
 * @param content
 * @return
 */
ListeTQ createListeProcessusFromString(const std::string &content);


/**
 *
 * @param input
 * @return
 */
bool isValidInlineCSV(const std::string& input);


/**
 *
 * @param filepath
 * @return
 */
bool isValidCSVFile(const char* filepath);