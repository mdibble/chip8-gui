#include "shader.h"

void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

Shader::Shader() {
    this->vertPath = "";
    this->fragPath = "";
}

Shader::Shader(const char* vertPath, const char* fragPath) {
    this->vertPath = vertPath;
    this->fragPath = fragPath;
}

void Shader::init() {
    std::string vertexCode, fragmentCode;
    std::ifstream vertFile, fragFile;
    vertFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertFile.open(this->vertPath);
        fragFile.open(this->fragPath);

        std::stringstream vertShaderStream, fragShaderStream;

        vertShaderStream << vertFile.rdbuf();
        fragShaderStream << fragFile.rdbuf();

        vertFile.close();
        fragFile.close();

        vertexCode = vertShaderStream.str(); 
        fragmentCode = fragShaderStream.str();
    }
    catch (std::ifstream::failure &err) {
        std::cout << "Error: file couldn't be read" << std::endl;
    }
    const char* vertexShader = vertexCode.c_str();
    const char* fragmentShader = fragmentCode.c_str();
   
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShader, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShader, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    this->id = glCreateProgram();
    glAttachShader(this->id, vertex);
    glAttachShader(this->id, fragment);
    glLinkProgram(this->id);
    checkCompileErrors(this->id, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(this->id); 
}
