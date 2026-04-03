add_library(ImSequencer STATIC
        ImSequencer.cpp
)

target_include_directories(ImSequencer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ImSequencer PUBLIC imgui)
