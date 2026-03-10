cmake_minimum_required(VERSION 3.31)
project(AbsolutOrdor)

set(CMAKE_CXX_STANDARD 20)

include_directories(core)
add_compile_options(-Wall -Wextra -Werror)

add_executable(
        #Executable Name
        Tests

        # Main
        Tests/Test_Core.cpp

        # Structures
        core/data/struct/liste.c
        core/data/struct/liste_tq.c
        core/data/struct/processus.c
        core/data/struct/resultat.c

        #Process
        core/process/Algo/sjf.c
        core/process/AlgoController.cpp

        #Utilitaires
        core/process/utilities/csv_reader.c
        core/process/Algo/fifo.c
)

#add_executable(
#        CLI
#        cli/main.cpp
#
#        cli/MainCli.cpp
#)

enable_testing()


add_test(NAME CoreTests COMMAND Tests)
