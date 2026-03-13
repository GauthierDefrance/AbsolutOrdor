cmake_minimum_required(VERSION 3.20)
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
        core/data/struct/ExecutionTimeline.c
        core/data/struct/file.c
        core/data/struct/file.h
        core/data/struct/ProcessusIterator.c
        core/data/struct/ProcessusIterator.h

        #Process
        core/process/AlgoController.cpp
        core/process/AlgoController.h

        #Utilitaires
        core/process/utilities/csv_reader.c

        #Algo
        core/process/Algo/fifo.c
        core/process/Algo/fifo.h
)

#add_executable(
#        CLI
#        cli/main.cpp
#
#        cli/MainCli.cpp
#)

enable_testing()


add_test(NAME CoreTests COMMAND Tests)
