#include <charconv>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>


enum class Screen { Select, Transition, Results };

constexpr double transition_duration = 10.0;

struct AppState {
    Screen screen = Screen::Select;
    std::string csv_path;
    double transition_start = 0.0;
};

static void DrawGui(AppState& s) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("AbsolutOrdor", nullptr, ImGuiWindowFlags_NoDecoration);
    switch (s.screen) {
        case Screen::Select: {
            ImGui::Text("Select");
            if (ImGui::Button("Choix de processus")) {
            }
            if (ImGui::Button("Choix d'algorithmes")) {
            }
            if (ImGui::Button("Lancer la Simulation")) {
                s.transition_start = glfwGetTime();
                s.screen = Screen::Transition;
            }
        }
            break;
        case Screen::Transition:
            ImGui::Text("Transition");
            ImGui::Text("time : %f",glfwGetTime()-s.transition_start);
            if (ImGui::Button("Debug")) s.screen = Screen::Select;
            if (glfwGetTime()-s.transition_start >= transition_duration) {
                s.screen = Screen::Results;
            }
            break;
        case Screen::Results:
            ImGui::Text("Results");
            if (ImGui::Button("Retour")) s.screen = Screen::Select;
            break;
    default: ;
    }
    ImGui::End();
}

int main() {
    // 1. Setup GLFW
    if (!glfwInit()) return 1;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "AbsolutOrdor", nullptr, nullptr);
    if (!window) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // 2. Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    AppState myApp;

    // 3. Boucle principale
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        DrawGui(myApp);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Nettoyage
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}