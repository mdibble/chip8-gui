#pragma once

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
private:
public:
    Shader(const char* vertPath, const char* fragPath);
    void use();
    unsigned int id;
};
