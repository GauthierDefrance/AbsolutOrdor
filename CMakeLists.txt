cmake_minimum_required(VERSION 3.31)
project(AbsolutOrdor)

# Active le support C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Active les tests
enable_testing()

# Ajoute le sous-dossier Src (avec son propre CMakeLists.txt)
add_subdirectory(Src)
