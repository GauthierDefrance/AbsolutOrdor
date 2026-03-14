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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Inclure les dossiers
include_directories(core Tests cli gui)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# --- 1. Recherche de Qt6 (Version Ultra-Stricte) ---

# On récupère le chemin passé par l'utilisateur.
# Si l'utilisateur passe C:/Qt/6.10.4 (qui n'existe pas), find_package DOIT échouer.
find_package(Qt6 REQUIRED COMPONENTS Widgets
        HINTS "${CMAKE_PREFIX_PATH}"
        NO_DEFAULT_PATH)

# --- 2. Options de compilation ---
add_compile_options(-Wall -Wextra -Werror)



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
)



# --- 3. Executables ---
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
)

#add_executable(CLI
#        ${CORE_SOURCES}
#        cli/main.cpp  # Assure-toi d'avoir un main.cpp dans le dossier cli
#        cli/ConsoleStructPrinter.cpp
#)

add_executable(GUI WIN32 MACOSX_BUNDLE
        ${CORE_SOURCES}
        gui/main.cpp
        gui/MainGui.cpp
)

target_link_libraries(GUI PRIVATE Qt6::Widgets)

# --- 4. Déploiement et Diagnostics ---

get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

message(STATUS "----------------------------------------------------")
message(STATUS "CONFIRMATION : Qt utilisé depuis : ${_qt_bin_dir}")
message(STATUS "----------------------------------------------------")

# --- 3. Déploiement spécifique par OS ---

get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

if(WIN32)
    # --- WINDOWS : windeployqt ---
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
    # --- MACOS : macdeployqt ---
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}" NO_DEFAULT_PATH)
    if(MACDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET GUI POST_BUILD
                # Note : On donne le chemin du .app (Bundle), pas du binaire seul
                COMMAND "${MACDEPLOYQT_EXECUTABLE}" "$<TARGET_BUNDLE_DIR:GUI>" -verbose=0
                COMMENT "macOS: Création du bundle .app avec macdeployqt..."
        )
    endif()

elseif(UNIX AND NOT APPLE)
    # --- LINUX : RPATH ---
    # Sur Linux, on demande à l'exécutable de chercher les libs dans le dossier de Qt
    # que l'on a passé en argument. C'est idéal pour le développement.

    # On récupère le dossier 'lib' de Qt
    get_filename_component(_qt_lib_dir "${_qt_bin_dir}/../lib" ABSOLUTE)

    # On inscrit ce chemin dans l'exécutable
    set_target_properties(GUI PROPERTIES
            INSTALL_RPATH "${_qt_lib_dir}"
            BUILD_WITH_INSTALL_RPATH TRUE
    )
    message(STATUS "Linux: RPATH configuré vers ${_qt_lib_dir}")
endif()

enable_testing()
add_test(NAME CoreTests COMMAND Tests)