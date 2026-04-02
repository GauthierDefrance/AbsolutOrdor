#include "MainGUI.h"

double transition_duration = 10.0;
char bufferCSVManuel[1024] = "";
int quantumRR = 2;

MainGUI& MainGUI::getInstance() {
    static MainGUI instance;
    return instance;
}

MainGUI::MainGUI() = default;

MainGUI::~MainGUI() = default;


std::string statsTimeline(const ExecutionTimeline *tl) {
    if (!tl) return "";

    double attenteMoy          = attenteMoyProcessus((ExecutionTimeline*)tl);
    double tempsRestitutionMoy = restitutionMoyProcessus((ExecutionTimeline*)tl);
    double reponseMoy          = tempRepMoyProcessus((ExecutionTimeline*)tl);
    double cpuUtil             = tauxOccupationCPU((ExecutionTimeline*)tl);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << std::left << std::setw(25) << "Temps d'attente moyen"   << ": " << attenteMoy          << "\n";
    ss << std::left << std::setw(25) << "Temps restitution moyen" << ": " << tempsRestitutionMoy << "\n";
    ss << std::left << std::setw(25) << "Temps de reponse moyen"  << ": " << reponseMoy          << "\n";
    ss << std::left << std::setw(25) << "Taux d'occupation CPU"   << ": " << cpuUtil             << " %\n";

    return ss.str();
}

void DrawGui(AppState& s) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("AbsolutOrdor", nullptr, ImGuiWindowFlags_NoDecoration);
    switch (s.screen) {
        case Screen::Select: {
            ImGui::Text("Select");
            if (ImGui::Button("Choix de processus")) {
                ImGui::OpenPopup("Choix de processus");
            }

            if (ImGui::Button("Choix d'algorithmes")) {
                ImGui::OpenPopup("Choix d'algorithmes");
            }

            if (AlgoController::CurrentAlgorithmNeedConfigChoice() && ImGui::Button("ConfigChoice")) {
                ImGui::OpenPopup("ConfigChoice");
            }

            if (ImGui::Button("Lancer la Simulation")&&AlgoController::canRunAlgorithm()) {
                s.transition_start = glfwGetTime();
                s.screen = Screen::Transition;
                AlgoController::runAlgorithm();
            }


            if (ImGui::BeginPopup("Choix de processus")) {
                bool flag_manuellement = false;
                bool flag_OpenCSV = false;
                ImGui::Text("Choix de processus");
                if (ImGui::Button("Ouvrir un CSV")) {
                    AlgoController::getInstance().setCSV(const_cast<char *>("../Tests/input.csv"));
                    ImGui::CloseCurrentPopup();
                    flag_OpenCSV = true;
                }
                if (ImGui::Button("manuellement")) {
                    ImGui::CloseCurrentPopup();
                    flag_manuellement = true;
                }
                ImGui::EndPopup();
                if (flag_manuellement)ImGui::OpenPopup("Choix de processus manuel");

                if (flag_OpenCSV) {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.flags = ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog(
                        "OpenCSV",
                        "Choisir un fichier CSV",
                        ".csv",
                        config
                    );
                }

            }

            ImGui::SetNextWindowSize(ImVec2(700, 450), ImGuiCond_Once);
            if (ImGuiFileDialog::Instance()->Display("OpenCSV")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
                    AlgoController::getInstance().setCSV(const_cast<char*>(path.c_str()));
                }
                ImGuiFileDialog::Instance()->Close();
            }


            if (ImGui::BeginPopup("Choix de processus manuel")) {
                ImGui::Text("processus manuel");
                ImGui::InputText(" ", bufferCSVManuel, IM_ARRAYSIZE(bufferCSVManuel));
                if (ImGui::Button("Enregistrer")) {
                    ImGui::CloseCurrentPopup();
                    AlgoController::getInstance().setContentCSV(bufferCSVManuel);
                }
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("Choix d'algorithmes")) {
                ImGui::Text("Choix d'algorithmes");
                for (std::string name : AlgoController::ALGO) {
                    if (ImGui::Button(name.data())) {
                        AlgoController::selectAlgorithm(name);
                        ImGui::CloseCurrentPopup();
                    }
                }
                ImGui::EndPopup();
            }

            if (ImGui::BeginPopup("ConfigChoice")) {
                ImGui::InputInt(" ", &quantumRR);
                if (quantumRR<1)quantumRR=1;
                if (ImGui::Button("Enregistrer")) {
                    ImGui::CloseCurrentPopup();
                    AlgoController::selectAlgorithm(AlgoController::getCurrentAlgorithmName(),{quantumRR});
                }
                ImGui::EndPopup();
            }

        }
            break;

        case Screen::Transition:
            ImGui::Text("Transition");
            ImGui::Text("time : %f",glfwGetTime()-s.transition_start);
            if (ImGui::Button("Skip")) s.screen = Screen::Results;
            if (glfwGetTime()-s.transition_start >= transition_duration) {
                s.screen = Screen::Results;
            }
            break;

        case Screen::Results:
            ImGui::Text("Results");
            if (AlgoController::CurrentAlgorithmNeedConfigChoice())ImGui::Text("%s%d",AlgoController::getCurrentAlgorithmName().c_str(),AlgoController::getCurrentAlgorithmConfig().quantumRR);
            else ImGui::Text("%s",AlgoController::getCurrentAlgorithmName().c_str());
            ImGui::Text("%s", statsTimeline(AlgoController::getExecutionTimeline()).c_str());

            if (ImGui::Button("Sauvegardez")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.fileName = "resultats.csv";
                config.flags = ImGuiFileDialogFlags_Modal;
                ImGuiFileDialog::Instance()->OpenDialog("SaveCSV","Enregistrer le fichier CSV",".csv",config);
            }

            if (ImGui::Button("Retour")) s.screen = Screen::Select;


            ImGui::SetNextWindowSize(ImVec2(700, 450), ImGuiCond_Once);
            if (ImGuiFileDialog::Instance()->Display("SaveCSV")){
                if (ImGuiFileDialog::Instance()->IsOk()){
                    std::string path = ImGuiFileDialog::Instance()->GetFilePathName();

                    if (!path.ends_with(".csv"))path += ".csv";
                    exportStatsCSV(AlgoController::getExecutionTimeline(),AlgoController::getCurrentAlgorithmName().c_str(),AlgoController::getCurrentAlgorithmConfig(),path,false);
                }
                ImGuiFileDialog::Instance()->Close();
            }

            break;
    default: ;
    }
    ImGui::End();
}

int MainGUI::run() {
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