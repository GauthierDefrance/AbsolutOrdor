cmake_minimum_required(VERSION 3.20)
project(AbsolutOrdor)

# --- 1. CONFIGURATION STANDARD ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 2. DÉPENDANCES EXTERNES ---
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(glfw URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip)
FetchContent_MakeAvailable(glfw)

set(IMGUI_DIR "libs/imgui")
add_subdirectory(${IMGUI_DIR})

# --- 3. DÉFINITION DES SOURCES ---

# Logique métier pure (C et C++)
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

# Utilitaires partagés entre CLI et Tests
set(UTILS_SOURCES
        cli/ConsoleStructPrinter.cpp
        core/process/utilities/string_parser.cpp
        core/process/utilities/string_parser.h
        core/process/utilities/csv_writer.cpp
        core/process/utilities/csv_writer.h
        core/process/utilities/TimelineStatsCalculator.c
)

# Fichiers de tests
set(TEST_FILES
        Tests/Test_Core.cpp
        Tests/Test_Liste.cpp
        Tests/Test_ListeTQ.cpp
        Tests/Test_Processus.cpp
        Tests/Test_CSV.cpp
        Tests/Test_Timeline.cpp
        Tests/Test_Algo.cpp
        Tests/Test_Inline.cpp
)

# --- 4. INCLUSIONS ---
include_directories(core Tests cli libs ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# --- 5. CIBLES (EXECUTABLES) ---

# Cible CLI
add_executable(CLI ${CORE_SOURCES} ${UTILS_SOURCES} cli/main.cpp cli/MainCli.cpp)
target_compile_options(CLI PRIVATE -Wall -Wextra -Werror) # <--- ICI

# Cible GUI
add_executable(GUI ${CORE_SOURCES} ${UTILS_SOURCES} gui/main.cpp
        gui/MainGUI.cpp
        gui/MainGUI.h
        gui/ExecutionTimelineSequencer.cpp
        gui/ExecutionTimelineSequencer.h
)
target_link_libraries(GUI imgui glfw OpenGL::GL)
target_compile_options(GUI PRIVATE -Wall -Wextra -Werror) # <--- ICI

# Cible TESTS
add_executable(Tests ${CORE_SOURCES} ${UTILS_SOURCES} ${TEST_FILES})
target_compile_options(Tests PRIVATE -Wall -Wextra -Werror)

# --- 6. CONFIGURATION DES TESTS ---
enable_testing()
add_test(NAME CoreTests COMMAND Tests)