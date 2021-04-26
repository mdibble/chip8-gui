#include "renderer.h"

Renderer::Renderer() {
    this->positions[0] =  1.0f;    this->positions[1] =  1.0f;    this->positions[2] = 1.0f;   this->positions[3] = 1.0f;
    this->positions[4] =  1.0f;    this->positions[5] = -1.0f;    this->positions[6] = 1.0f;   this->positions[7] = 0.0f;
    this->positions[8] = -1.0f;    this->positions[9] = -1.0f;    this->positions[10]= 0.0f;   this->positions[11]= 0.0f;
    this->positions[12]= -1.0f;    this->positions[13]=  1.0f;    this->positions[14]= 0.0f;   this->positions[15]= 1.0f;

    this->indices[0] = 0;   this->indices[1] = 1;   this->indices[2] = 3;
    this->indices[3] = 1;   this->indices[4] = 2;   this->indices[5] = 3;

    this->shader = Shader("src/shaders/chip8.vert", "src/shaders/chip8.frag");
}

void Renderer::init() {
    this->shader.init();
}

void Renderer::render() {
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));
    glGenTextures(1, &(this->texture));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->positions), this->positions, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);
    
    this->generateCanvas();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, this->canvas);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    this->shader.use();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::clearCanvas() {
    for (int i = 0; i < 32; i += 1) {
        for (int j = 0; j < 64; j += 1) {
            this->display[i][j] = false;
        }
    }
}

void Renderer::generateCanvas() {
    for (int i = 0; i < 32; i += 1) {
        for (int j = 0; j < 64; j += 1) {
            unsigned char val = (this->display[31 - i][j] == true ? 255 : 0);
            this->canvas[(i * 64 * 3) + ((j * 3) + 0)] = val;
            this->canvas[(i * 64 * 3) + ((j * 3) + 1)] = val;
            this->canvas[(i * 64 * 3) + ((j * 3) + 2)] = val;
        }
    }
}
