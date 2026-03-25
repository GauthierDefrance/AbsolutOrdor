cmake_minimum_required(VERSION 3.20)
project(AbsolutOrdor)

# -----------------------
# C++ Standard
# -----------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------
# Inclure les dossiers
# -----------------------
include_directories(core Tests cli)

# -----------------------
# Options de compilation
# -----------------------
add_compile_options(-Wall -Wextra -Werror)

# -----------------------
# Sources Core
# -----------------------
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

# -----------------------
# Executable Tests CLI
# -----------------------
add_executable(Tests
        ${CORE_SOURCES}
        Tests/Test_Core.cpp
        Tests/Test_Liste.cpp
        Tests/Test_ListeTQ.cpp
        Tests/Test_Processus.cpp
        Tests/Test_CSV.cpp
        Tests/Test_Timeline.cpp
        Tests/Test_Algo.cpp
        cli/ConsoleStructPrinter.cpp
        core/process/utilities/TimelineStatsCalculator.c
        core/process/utilities/TimelineStatsCalculator.h
)

add_executable(CLI
        ${CORE_SOURCES}
        cli/main.cpp
        cli/MainCli.cpp
        cli/ConsoleStructPrinter.cpp
        core/process/utilities/TimelineStatsCalculator.c
        core/process/utilities/TimelineStatsCalculator.h
)

# -----------------------
# DLLs MinGW pour CLI (Windows)
# -----------------------
if(WIN32)
    target_link_options(CLI PRIVATE
            -static
            -static-libgcc
            -static-libstdc++
    )
endif()

# -----------------------
# Tests CTest
# -----------------------
enable_testing()
add_test(NAME CoreTests COMMAND Tests)