#pragma once
#include <charconv>
#include <iomanip>
#include <ios>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

#include "process/AlgoController.h"

extern "C" {
#include "process/AlgoController.h"
#include "data/struct/liste.h"
#include "data/struct/liste_tq.h"
#include "data/struct/processus.h"
#include "data/struct/ExecutionTimeline.h"
#include "process/utilities/csv_reader.h"
#include "process/utilities/TimelineStatsCalculator.h"
}
enum class Screen { Select, Transition, Results };
struct AppState {
    Screen screen = Screen::Select;
    double transition_start = 0.0;
};
class MainGUI {
    public:
        static MainGUI& getInstance();
        int run();
        ~MainGUI();
    private:
        // Constructeur de classe
        MainGUI();
};