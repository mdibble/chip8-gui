#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "uiState.h"

class UI {
private:
public:
    UI();
    void init();
    void destroy();
    void render();
    bool isRunning();
    GLFWwindow* window;
    UIState state;
};
