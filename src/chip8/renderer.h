#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

class Renderer {
private:
public:
    Renderer();
    unsigned int VBO, EBO;
    float positions[20];
    unsigned int indices[6];
    bool canvas[32][64];
    void render();
    void assignShader(const char* fragPath, const char* vertPath);
    void clearCanvas();
};
