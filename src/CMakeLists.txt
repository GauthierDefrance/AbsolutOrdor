cmake_minimum_required(VERSION 3.20)
project(AbsolutOrdor)

# --- INSTRUCTIONS ---
# cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64"
# ou
# cmake -B "cmake-build-debug" -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64"
#
# Sur Clion allez dans Ctrl + Alt + s, Cmake,
# et ajouter ceci dans Cmake options :
# -DCMAKE_PREFIX_PATH="C:/Qt/6.10.1/mingw_64"
#
# cmake --build build
# --------------------

# -----------------------
# C++ Standard
# -----------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------
# Inclure les dossiers
# -----------------------
include_directories(core Tests cli gui)

# -----------------------
# CMake automatique Qt
# -----------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -----------------------
# Option pour désactiver Qt
# -----------------------
option(DISABLE_QT "Disable Qt GUI support" OFF)

# -----------------------
# Recherche optionnelle de Qt6
# -----------------------
set(QT6_FOUND FALSE)
if(NOT DISABLE_QT)
    find_package(Qt6 COMPONENTS Widgets QUIET)
    if(Qt6_FOUND)
        set(QT6_FOUND TRUE)
        message(STATUS "Qt6 trouvé, GUI activée")
    else()
        message(WARNING "Qt6 non trouvé, GUI désactivée")
    endif()
else()
    message(STATUS "Qt GUI désactivée par option DISABLE_QT")
endif()

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

add_executable(
        CLI
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
# Executable GUI (Qt seulement si trouvé)
# -----------------------
if(QT6_FOUND)
    add_executable(GUI WIN32 MACOSX_BUNDLE
            ${CORE_SOURCES}
            gui/main.cpp
            gui/MainGui.cpp
    )
    target_link_libraries(GUI PRIVATE Qt6::Widgets)

    # -----------------------
    # Déploiement spécifique par OS
    # -----------------------
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

    if(WIN32)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}" NO_DEFAULT_PATH)
        if(WINDEPLOYQT_EXECUTABLE)
            add_custom_command(TARGET GUI POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E env "PATH=${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                    --no-compiler-runtime
                    "$<TARGET_FILE:GUI>"
                    COMMENT "Windows: Déploiement des DLL Qt..."
            )
        endif()

    elseif(APPLE)
        find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}" NO_DEFAULT_PATH)
        if(MACDEPLOYQT_EXECUTABLE)
            add_custom_command(TARGET GUI POST_BUILD
                    COMMAND "${MACDEPLOYQT_EXECUTABLE}" "$<TARGET_BUNDLE_DIR:GUI>" -verbose=0
                    COMMENT "macOS: Création du bundle .app avec macdeployqt..."
            )
        endif()

    elseif(UNIX AND NOT APPLE)
        get_filename_component(_qt_lib_dir "${_qt_bin_dir}/../lib" ABSOLUTE)
        set_target_properties(GUI PROPERTIES
                INSTALL_RPATH "${_qt_lib_dir}"
                BUILD_WITH_INSTALL_RPATH TRUE
        )
        message(STATUS "Linux: RPATH configuré vers ${_qt_lib_dir}")
    endif()
endif()

# -----------------------
# Tests CTest
# -----------------------
enable_testing()
add_test(NAME CoreTests COMMAND Tests)