#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

class Renderer {
private:
public:
    Renderer();
    Shader shader;
    unsigned int VBO, EBO, texture;
    float positions[16];
    unsigned int indices[6];
    bool display[32][64];
    unsigned char canvas[64 * 32 * 3];
    void render();
    void init();
    void clearCanvas();
    void generateCanvas();
};
