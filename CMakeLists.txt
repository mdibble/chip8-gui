cmake_minimum_required(VERSION 3.20)
project(chip8-imgui)

# GLAD
add_library(glad STATIC
    vendor/glad/src/glad.c
)
target_include_directories(glad PUBLIC vendor/glad/include)

# GLFW
add_subdirectory(vendor/glfw)
target_include_directories(glad PUBLIC vendor/glfw/include)

# ImGui
add_library(imgui STATIC
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui
    PUBLIC
    vendor/glfw/include
    vendor/glad/include
    vendor/imgui
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(chip8-imgui ${SOURCES})

target_link_libraries(chip8-imgui
    glad
    glfw
    imgui
)
