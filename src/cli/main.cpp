#include "MainCli.h"


/**
 * Point d'entrée du MainCLI
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
    MainCli cli = MainCli::getInstance();
    return cli.run(argc, argv);
}