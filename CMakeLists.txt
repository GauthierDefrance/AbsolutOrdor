cmake_minimum_required(VERSION 4.1)
project(AbsolutOrdor)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        AbsolutOrdor
        src/Tests/Test_Core.cpp
)

add_executable(
        Tests
        src/Tests/Test_Core.cpp
        src/core/data/struct/resultat.h
        src/core/data/struct/liste.h
        src/core/data/struct/liste_tq.h
        src/core/data/struct/processus.c
        src/core/data/struct/resultat.c
        src/core/data/struct/liste.c
        src/core/data/struct/liste_tq.c
        src/core/data/struct/processus.h
        src/core/process/utilities/csv_reader.c
        src/core/process/utilities/csv_reader.h
        src/core/process/AlgoController.cpp
        src/core/process/AlgoController.h
)