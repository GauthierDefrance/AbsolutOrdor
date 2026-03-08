cmake_minimum_required(VERSION 4.1)
project(AbsolutOrdor)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        AbsolutOrdor
        main.cpp
        Tests/Test_Core.cpp
)

add_executable(
        Tests
        Tests/Test_Core.cpp
        C/Core/resultat.h
        C/Core/liste.h
        C/Core/liste_tq.h
        C/Core/processus.c
        C/Core/resultat.c
        C/Core/liste.c
        C/Core/liste_tq.c
        C/Core/processus.h
        C/Core/csv_reader.c
        C/Core/csv_reader.h
        C/Core/AlgoController.cpp
        C/Core/AlgoController.h
)