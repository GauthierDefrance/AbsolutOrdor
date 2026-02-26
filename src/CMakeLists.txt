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
        C/Core/structure.h
        C/Core/resultat.h
        C/Core/data.h
        C/Core/liste_generique.h
        C/Core/liste_tq.h
        C/Core/processus.c
        C/Core/resultat.c
        C/Core/data.c
        C/Core/liste_generique.c
        C/Core/liste_tq.c
        C/Core/processus.h
)