cmake_minimum_required(VERSION 3.20)

project(AbsolutOrdor LANGUAGES C CXX)

# --- Standards ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Pour éviter le warning FetchContent CMP0135 (CMake récent)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

# --- Dépendances ---
find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(glfw
        URL https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

set(IMGUI_DIR "libs/imgui")
add_subdirectory(${IMGUI_DIR})

# --- Sources ---
set(CORE_SOURCES
        core/data/struct/liste.c
        core/data/struct/liste_tq.c
        core/data/struct/processus.c
        core/data/struct/ExecutionTimeline.c
        core/data/struct/file.c
        core/data/struct/ProcessusIterator.c
        core/process/AlgoController.cpp
        core/process/Algo/fifo.c
        core/process/Algo/sjf.c
        core/process/Algo/sjrf.c
        core/process/Algo/algo_commun.c
        core/process/Algo/lottery_scheduling.c
        core/process/Algo/rrn.c
)

set(UTILS_SOURCES
        cli/ConsoleStructPrinter.cpp
        core/process/utilities/string_parser.cpp
        core/process/utilities/string_parser.h
        core/process/utilities/csv_writer.cpp
        core/process/utilities/csv_writer.h
        core/process/utilities/TimelineStatsCalculator.c
)

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

# --- Includes (par target, portable) ---
set(PROJ_INC
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/Tests
        ${CMAKE_CURRENT_SOURCE_DIR}/cli
        ${CMAKE_CURRENT_SOURCE_DIR}/libs
        ${CMAKE_CURRENT_SOURCE_DIR}/${IMGUI_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/${IMGUI_DIR}/backends
)

# --- Warnings stricts : GCC/Clang vs MSVC ---
function(enable_strict_warnings target)
    target_compile_options(${target} PRIVATE
            # MSVC
            $<$<COMPILE_LANG_AND_ID:C,MSVC>:/W4 /WX>
            $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/W4 /WX>

            # GCC/Clang (Linux/macOS + MinGW/Clang)
            $<$<AND:$<COMPILE_LANGUAGE:C>,$<NOT:$<C_COMPILER_ID:MSVC>>>:-Wall -Wextra -Werror>
            $<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-Wall -Wextra -Werror>
    )
endfunction()

# --- Targets ---
add_executable(CLI
        ${CORE_SOURCES}
        ${UTILS_SOURCES}
        cli/main.cpp
        cli/MainCli.cpp
)
target_include_directories(CLI PRIVATE ${PROJ_INC})
enable_strict_warnings(CLI)

add_executable(GUI
        ${CORE_SOURCES}
        ${UTILS_SOURCES}
        gui/main.cpp
        gui/MainGUI.cpp
        gui/MainGUI.h
        gui/ExecutionTimelineSequencer.cpp
        gui/ExecutionTimelineSequencer.h
)
target_include_directories(GUI PRIVATE ${PROJ_INC})
target_link_libraries(GUI PRIVATE imgui glfw OpenGL::GL)
enable_strict_warnings(GUI)

add_executable(Tests
        ${CORE_SOURCES}
        ${UTILS_SOURCES}
        ${TEST_FILES}
)
target_include_directories(Tests PRIVATE ${PROJ_INC})
enable_strict_warnings(Tests)

# --- Tests CTest ---
enable_testing()
add_test(NAME CoreTests COMMAND Tests)

# --- Documentation Doxygen ---
option(BUILD_DOCS "Générer la documentation Doxygen" ON)

find_package(Doxygen QUIET)

if (BUILD_DOCS AND DOXYGEN_FOUND)
    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Génération de la documentation Doxygen"
            VERBATIM
    )
else()
    message(STATUS "Doxygen non trouvé ou BUILD_DOCS=OFF : cible 'docs' non générée")
endif()