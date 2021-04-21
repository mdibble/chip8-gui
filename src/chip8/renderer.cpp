#include "renderer.h"

Renderer::Renderer() {
    this->positions[0] =  1.0f;  this->positions[1] =  1.0f;
    this->positions[2] =  1.0f;  this->positions[3] = -1.0f;
    this->positions[4] = -1.0f;  this->positions[5] = -1.0f;
    this->positions[6] = -1.0f;  this->positions[7] =  1.0f;

    this->indices[0] = 0;   this->indices[1] = 1;   this->indices[2] = 3;
    this->indices[3] = 1;   this->indices[4] = 2;   this->indices[5] = 3;
}

void Renderer::render() {
    glGenBuffers(1, &(this->VBO));
    glGenBuffers(1, &(this->EBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->positions), this->positions, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
