add_library(imgui STATIC
        imgui.cpp
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
        ../imGuiFileDialog/ImGuiFileDialog.cpp
)

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}                 # <-- contient imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/../imGuiFileDialog
)

# On lie GLFW et OpenGL à la lib imgui
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)