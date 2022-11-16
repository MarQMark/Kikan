#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmentSource = loadShaderSource(fragmentPath);

    _id = glCreateProgram();
    glAttachShader(_id, compileShader(GL_VERTEX_SHADER, vertexSource));
    glAttachShader(_id, compileShader(GL_FRAGMENT_SHADER, fragmentSource));
    glLinkProgram(_id);

    bind();
}

std::string Shader::loadShaderSource(const std::string& path) {
    std::stringstream ss;
    std::string line;
    std::ifstream source(path);

    while(std::getline(source, line))
        ss << line << std::endl;

    source.close();

    return ss.str();
}

int Shader::compileShader(GLenum type, const std::string& source) {
    int id = glCreateShader(type);
    const char *c_str = source.c_str();
    glShaderSource(id, 1, &c_str, nullptr);
    glCompileShader(id);

    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    std::cout << "[SHADER COMPILE] "
            << ((type == GL_VERTEX_SHADER) ? "VertexShader" : "FragmentShader")
            << "  Status " << status << std::endl;

    return id;
}

void Shader::bind() const {
    glUseProgram(_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

int Shader::uniform_location(const std::string& name) {
    if(_uniforms.count(name))
        return _uniforms[name];

    GLint loc = glGetUniformLocation(_id, name.c_str());
    _uniforms[name] = loc;
    if(loc == -1)
        std::cout << "[ERROR] Uniform " << name << " not found" << std::endl;

    return loc;
}

void Shader::uniform1li(const std::string &name, int x) {
    GLint loc = uniform_location(name);
    if(loc != -1)
        glUniform1i(loc, x);
}

void Shader::uniform1lf(const std::string &name, float x) {
    GLint loc = uniform_location(name);
    if(loc != -1)
        glUniform1f(loc, x);
}


