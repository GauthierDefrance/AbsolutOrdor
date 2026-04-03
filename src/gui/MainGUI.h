#pragma once
#include <charconv>
#include <iomanip>
#include <ios>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"
#include <GLFW/glfw3.h>
#include <string>
#include "process/AlgoController.h"
#include "process/utilities/csv_writer.h"
/**
 * @file MainGUI.h
 * @brief Classe servant à lancer l'application graphique
 */



extern "C" {
#include "process/AlgoController.h"
#include "data/struct/liste.h"
#include "data/struct/liste_tq.h"
#include "data/struct/processus.h"
#include "data/struct/ExecutionTimeline.h"
#include "process/utilities/TimelineStatsCalculator.h"
}

/**
 * Enum gardant en mémoire les différents états possible de l'application
 */
enum class Screen { Select, Transition, Results };

/**
 * Structure servant à connaître l'état de l'application
 */
struct AppState {
    Screen screen = Screen::Select;
    double transition_start = 0.0;
};

/**
 * @class MainGUI
 * @brief Classe servant à lancer l'application graphique
 */
class MainGUI {
    public:
        static MainGUI& getInstance();
        int run();
        ~MainGUI();
    private:
        // Constructeur de classe
        MainGUI();
};