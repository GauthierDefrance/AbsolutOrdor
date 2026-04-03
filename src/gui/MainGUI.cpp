#include "MainGUI.h"

#include "ExecutionTimelineSequencer.h"

bool dejaClique = false;
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


static void DrawTimelineLegend(){
    auto legendItem = [](ImU32 col, const char* label){
        ImGui::ColorButton(label, ImGui::ColorConvertU32ToFloat4(col),ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop,ImVec2(14, 14));
        ImGui::SameLine();
        ImGui::TextUnformatted(label);
        ImGui::SameLine(0.0f, 14.0f);
    };

    ImGui::TextUnformatted("Legende :");
    ImGui::SameLine();

    // Couleurs identiques à celles utilisées pour les segments :
    legendItem(IM_COL32( 80, 200, 120, 255), "UC"); // vert
    legendItem(IM_COL32( 80, 140, 240, 255), "ES"); // bleu
    legendItem(IM_COL32(220, 170,  70, 255), "W");  // orange
}


void DisplayWindowSize(float size) {
    ImVec2 display = ImGui::GetIO().DisplaySize;
    ImVec2 win(display.x * size, display.y * size);
    ImGui::SetNextWindowSize(win, ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2((display.x - win.x) * 0.5f, (display.y - win.y) * 0.5f),ImGuiCond_Once);
}


void DisplayPopupSize(float size){
    ImVec2 display = ImGui::GetIO().DisplaySize;
    ImVec2 win(display.x * size, display.y * size);

    ImGui::SetNextWindowSize(win);
    ImGui::SetNextWindowPos(ImVec2((display.x - win.x) * 0.5f,(display.y - win.y) * 0.5f));
}

void globalTheme () {

    {
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowRounding = 8.0f;
        style.FrameRounding  = 6.0f;
        style.PopupRounding  = 6.0f;

        //style.WindowPadding  = ImVec2(12, 12);
        //style.ItemSpacing    = ImVec2(10, 8);

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg]  = ImVec4(0.12f,0.12f,0.12f,1.00f);
        colors[ImGuiCol_PopupBg]   = ImVec4(0.10f,0.10f,0.10f,1.00f);
    }

}

void setButtonRed(){
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.35f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 0.45f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.55f));
}

void setButtonGreen(){
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.35f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 1.0f, 0.0f, 0.45f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 0.55f));
}


void DrawGui(AppState& s) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("AbsolutOrdor", nullptr, ImGuiWindowFlags_NoDecoration);
    switch (s.screen) {
        case Screen::Select: {

            if (ImGui::BeginTable("MenuPrincipal", 3, ImGuiTableFlags_SizingStretchSame)) {

                bool openChoixProcessusPopup = false;
                bool openChoixAlgoPopup = false;
                bool openConfigPopup = false;

                float fullHeight = ImGui::GetContentRegionAvail().y;

                ImGui::TableNextColumn();
                if (AlgoController::listeProcessusIsEmpty())setButtonGreen();
                if (ImGui::Button("Choix de processus", ImVec2(-FLT_MIN, fullHeight))) {openChoixProcessusPopup = true;}
                if (AlgoController::listeProcessusIsEmpty())ImGui::PopStyleColor(3);

                ImGui::TableNextColumn();
                if (AlgoController::CurrentAlgorithmNeedConfigChoice()) {
                    setButtonGreen();
                    if (ImGui::Button("Choix d'algorithmes", ImVec2(-FLT_MIN, (fullHeight * 0.5f)-2))) {
                        openChoixAlgoPopup = true;
                    }
                    if (!dejaClique)ImGui::PopStyleColor(3);
                    if (ImGui::Button("Choix de configuration", ImVec2(-FLT_MIN, (fullHeight * 0.5f)-2))) {
                        openConfigPopup = true;
                    }
                    if (dejaClique)ImGui::PopStyleColor(3);
                }
                else {
                    dejaClique = false;
                    if (!AlgoController::getCurrentAlgorithmName().empty())setButtonGreen();
                    if (ImGui::Button("Choix d'algorithmes", ImVec2(-FLT_MIN, fullHeight))) {openChoixAlgoPopup = true;}
                    if (!AlgoController::getCurrentAlgorithmName().empty())ImGui::PopStyleColor(3);
                }

                ImGui::TableNextColumn();
                if (!AlgoController::canRunAlgorithm()) setButtonRed();

                if (ImGui::Button("Lancer la Simulation", ImVec2(-FLT_MIN, fullHeight)) && AlgoController::canRunAlgorithm()){
                    s.transition_start = glfwGetTime();
                    s.screen = Screen::Transition;
                    AlgoController::runAlgorithm();
                }
                if (!AlgoController::canRunAlgorithm())ImGui::PopStyleColor(3);

                ImGui::EndTable();

                if (openChoixProcessusPopup) ImGui::OpenPopup("Choix de processus");
                if (openChoixAlgoPopup) ImGui::OpenPopup("Choix d'algorithmes");
                if (openConfigPopup) ImGui::OpenPopup("Choix de configuration");
            }


            DisplayPopupSize(0.7f);
            if (ImGui::BeginPopup("Choix de processus")) {

                ImGui::Text("Choix de processus");
                ImGui::Separator();

                bool flag_manuellement = false;
                bool flag_OpenCSV = false;

                float fullH = ImGui::GetContentRegionAvail().y-4;

                if (ImGui::BeginTable("ChoixProcessusTable", 2, ImGuiTableFlags_SizingStretchSame)){

                    ImGui::TableNextColumn();
                    if (ImGui::Button("Ouvrir un CSV", ImVec2(-FLT_MIN, fullH))) {
                        ImGui::CloseCurrentPopup();
                        flag_OpenCSV = true;
                    }

                    ImGui::TableNextColumn();
                    if (ImGui::Button("Manuellement", ImVec2(-FLT_MIN, fullH))) {
                        ImGui::CloseCurrentPopup();
                        flag_manuellement = true;
                    }

                    ImGui::EndTable();
                }

                ImGui::EndPopup();

                if (flag_manuellement)ImGui::OpenPopup("Choix de processus manuel");

                if (flag_OpenCSV) {
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.flags = ImGuiFileDialogFlags_Modal;
                    ImGuiFileDialog::Instance()->OpenDialog("OpenCSV","Choisir un fichier CSV",".csv",config);
                }
            }

            DisplayPopupSize(0.7);
            if (ImGui::BeginPopup("Choix de processus manuel")) {

                ImGui::Text("Processus manuel : Meme convention que pour un fichier input CSV");
                ImGui::Separator();

                ImVec2 avail = ImGui::GetContentRegionAvail();
                ImVec2 popupSize = ImGui::GetWindowSize();
                float buttonHeight = popupSize.y * 0.07f;
                avail.y -= buttonHeight + 4.0f;

                ImGui::InputTextMultiline("##manuel",bufferCSVManuel,IM_ARRAYSIZE(bufferCSVManuel),avail);

                if (ImGui::Button("Enregistrer", ImVec2(-FLT_MIN, buttonHeight))) {
                    ImGui::CloseCurrentPopup();
                    AlgoController::getInstance().setContentCSV(bufferCSVManuel);
                }

                ImGui::EndPopup();
            }




            DisplayPopupSize(0.7f);
            if (ImGui::BeginPopup("Choix d'algorithmes")){
                ImGui::Text("Choix d'algorithmes");
                ImGui::Separator();

                int N = AlgoController::ALGO.size();
                int maxCols = 4;

                ImVec2 popup = ImGui::GetWindowSize();

                float minH = popup.y * 0.10f;
                float targetH = popup.y * 0.30;

                int columns = 1;

                while (columns < maxCols){
                    int rows = (N + columns - 1) / columns;
                    float availableH = popup.y / rows;
                    if (availableH < targetH) columns++;
                    else break;
                }

                int rowsFinal = (N + columns - 1) / columns;
                float finalH = popup.y / rowsFinal;
                if (finalH < minH) finalH = minH;
                if (finalH > targetH) finalH = targetH;
                if (ImGui::BeginTable("AlgoTable", columns,ImGuiTableFlags_SizingStretchSame)){
                    for (std::string_view name : AlgoController::ALGO){
                        ImGui::TableNextColumn();
                        bool found = false;
                        if (name == AlgoController::getCurrentAlgorithmName()) {
                            found = true;
                            setButtonGreen();
                        }
                        if (ImGui::Button(name.data(), ImVec2(-FLT_MIN, finalH))){
                            AlgoController::selectAlgorithm(name.data());
                            ImGui::CloseCurrentPopup();
                        }
                        if (found)ImGui::PopStyleColor(3);
                    }
                    ImGui::EndTable();
                }
                ImGui::EndPopup();
            }


            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopup("Choix de configuration")) {
                ImGui::InputInt(" /", &quantumRR);
                ImGui::SameLine();
                if (quantumRR<1)quantumRR=1;
                if (ImGui::Button("Enregistrer")) {
                    ImGui::CloseCurrentPopup();
                    AlgoController::selectAlgorithm(AlgoController::getCurrentAlgorithmName(),{quantumRR});
                    dejaClique = true;
                }
                ImGui::EndPopup();
            }

            DisplayWindowSize(0.9);
            if (ImGuiFileDialog::Instance()->Display("OpenCSV")) {
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string path = ImGuiFileDialog::Instance()->GetFilePathName();
                    AlgoController::getInstance().setCSV(const_cast<char*>(path.c_str()));
                }
                ImGuiFileDialog::Instance()->Close();
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

        case Screen::Results:{
            const ExecutionTimeline* tl = AlgoController::getExecutionTimeline();

            ImGui::SeparatorText("Execution Timeline");
            DrawTimelineLegend();
            static ExecutionTimelineSequencer sequencer;
            ImGui::NewLine();

            static const ExecutionTimeline* lastTL = nullptr;

            // (Re)charger la timeline si elle a changé
            if (tl && tl != lastTL) {
                sequencer.SetTimeline(tl);
                sequencer.SetCurrentFrame(sequencer.GetFrameMin()); // optionnel : reset au début
                lastTL = tl;
            }

            float customHeightsSum = 0.0f;

            float idealHeight = sequencer.GetItemCount() * 20.0f + 70.0f + customHeightsSum ;

            float maxHeight = ImGui::GetIO().DisplaySize.y * 0.70f;
            float availY = ImGui::GetContentRegionAvail().y;

            float timelineHeight = idealHeight;
            if (timelineHeight > maxHeight) timelineHeight = maxHeight;
            if (timelineHeight > availY)   timelineHeight = availY;

            ImGui::BeginChild("TimelineChild", ImVec2(0, timelineHeight), true, ImGuiWindowFlags_HorizontalScrollbar);
            sequencer.Draw(ImSequencer::SEQUENCER_CHANGE_FRAME);
            ImGui::EndChild();


            if (AlgoController::CurrentAlgorithmNeedConfigChoice())ImGui::Text("Results : %s%d", AlgoController::getCurrentAlgorithmName().c_str(),AlgoController::getCurrentAlgorithmConfig().quantumRR);
            else ImGui::Text("Results : %s", AlgoController::getCurrentAlgorithmName().c_str());
            ImGui::Separator();

            ImGui::Text("%s", statsTimeline(tl).c_str());

            if (ImGui::Button("Sauvegardez")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.fileName = "resultats.csv";
                config.flags = ImGuiFileDialogFlags_Modal;
                ImGuiFileDialog::Instance()->OpenDialog("SaveCSV","Enregistrer le fichier CSV",".csv",config);
            }

            if (ImGui::Button("Retour")) s.screen = Screen::Select;


            DisplayWindowSize(0.9);
            if (ImGuiFileDialog::Instance()->Display("SaveCSV")){
                if (ImGuiFileDialog::Instance()->IsOk()){
                    std::string path = ImGuiFileDialog::Instance()->GetFilePathName();

                    if (!path.ends_with(".csv"))path += ".csv";
                    exportStatsCSV(AlgoController::getExecutionTimeline(),AlgoController::getCurrentAlgorithmName().c_str(),AlgoController::getCurrentAlgorithmConfig(),path,false);
                }
                ImGuiFileDialog::Instance()->Close();
            }
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
    globalTheme();

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