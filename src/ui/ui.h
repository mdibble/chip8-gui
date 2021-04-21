#pragma once

#ifdef _WIN32
#include <Windows.h>
#elif __gnu_linux__
#include <byteswap.h>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "uiState.h"
#include "../chip8/chip8.h"
#include "../chip8/actions.h"

class UI {
private:
public:
    UI();
    void init(CHIP8* system);
    void destroy();
    void render();
    bool isRunning();
    GLFWwindow* window;
    UIState state;
    CHIP8* system;
};
