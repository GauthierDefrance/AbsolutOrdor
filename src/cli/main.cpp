#include "MainCli.h"


/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
    MainCli cli = MainCli::getInstance();
    cli.run(argc, argv);
}