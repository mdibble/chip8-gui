#pragma once

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Shader {
private:
    const char* vertPath;
    const char* fragPath;
public:
    Shader();
    Shader(const char* vertPath, const char* fragPath);
    void init();
    void use();
    unsigned int id;
};
