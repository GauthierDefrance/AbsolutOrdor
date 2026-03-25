cmake_minimum_required(VERSION 3.20)
project(AbsolutOrdor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Gestion des dépendances universelles
find_package(OpenGL REQUIRED)

# Téléchargement automatique de GLFW (pour que ça marche partout sans rien installer)
include(FetchContent)
FetchContent_Declare(glfw
        URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip
)
FetchContent_MakeAvailable(glfw)

# 2. Configuration ImGui
set(IMGUI_DIR "libs/imgui")
add_subdirectory(${IMGUI_DIR})

# 3. Chemins d'inclusion
include_directories(core Tests cli libs ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# 4. Sources communes (Logique métier)
set(CORE_SOURCES
        core/data/struct/liste.c
        core/data/struct/liste_tq.c
        core/data/struct/processus.c
        core/data/struct/ExecutionTimeline.c
        core/data/struct/file.c
        core/data/struct/ProcessusIterator.c
        core/process/AlgoController.cpp
        core/process/utilities/csv_reader.c
        core/process/Algo/fifo.c
        core/process/Algo/sjf.c
        core/process/Algo/sjrf.c
        core/process/Algo/algo_commun.c
        core/process/Algo/lottery_scheduling.c
        core/process/Algo/rrn.c
)

# 5. Cibles
add_executable(CLI ${CORE_SOURCES} cli/main.cpp)

add_executable(GUI ${CORE_SOURCES} gui/main.cpp)
# On lie ImGui, GLFW et OpenGL
target_link_libraries(GUI imgui glfw OpenGL::GL)

# 6. Tests
enable_testing()
add_executable(Tests ${CORE_SOURCES} Tests/Test_Core.cpp) # Ajoute tes autres fichiers tests ici
add_test(NAME CoreTests COMMAND Tests)