#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
private:
public:
    Renderer();
    unsigned int VBO, EBO;
    float positions[8];
    unsigned int indices[6];
    bool canvas[32][64];
    void render();
};
