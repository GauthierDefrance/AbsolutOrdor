cmake_minimum_required(VERSION 3.20)

add_library(ImGuiFileDialog STATIC
        ImGuiFileDialog.cpp
)

target_include_directories(ImGuiFileDialog PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Dépend de imgui
target_link_libraries(ImGuiFileDialog PUBLIC imgui)
