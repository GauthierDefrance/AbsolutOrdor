cmake_minimum_required(VERSION 4.1)
project(AbsolutOrdor)

set(CMAKE_CXX_STANDARD 20)

include_directories(core)

add_executable(
        #Executable Name
        Tests

        # Main
        Tests/Test_Core.cpp

        # Structures
        core/data/struct/liste.h
        core/data/struct/liste.c
        core/data/struct/liste_tq.h
        core/data/struct/liste_tq.c
        core/data/struct/processus.h
        core/data/struct/processus.c
        core/data/struct/resultat.h
        core/data/struct/resultat.c

        #Process
        core/process/Algo/sjf.c
        core/process/Algo/sjf.h
        core/process/AlgoController.h
        core/process/AlgoController.cpp

        #Utilitaires
        core/process/utilities/csv_reader.h
        core/process/utilities/csv_reader.c
        core/process/Algo/fifo.c
        core/process/Algo/fifo.h
)

add_executable(
        CLI
        cli/main.cpp

        cli/MainCli.cpp
        cli/MainCli.h
)