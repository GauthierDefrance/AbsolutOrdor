cmake_minimum_required(VERSION 4.1)
project(AbsolutOrdor)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        AbsolutOrdor
        main.cpp
)

add_executable(
        Tests
        C/structure2donnees/test_struct.c
        C/structure2donnees/struct.c
        C/structure2donnees/structure_processeur.h
)