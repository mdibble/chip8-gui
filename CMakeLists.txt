cmake_minimum_required(VERSION 3.20)
project(chip8-imgui)

# GLAD
add_library(glad STATIC
    lib/glad/src/glad.c
)
target_include_directories(glad PUBLIC lib/glad/include)

# GLFW
add_subdirectory(lib/glfw)
target_include_directories(glad PUBLIC lib/glfw/include)

# ImGui
add_library(imgui STATIC
    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui
    PUBLIC
    lib/glfw/include
    lib/glad/include
    lib/imgui
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(chip8-imgui ${SOURCES})

target_link_libraries(chip8-imgui
    glad
    glfw
    imgui
)
