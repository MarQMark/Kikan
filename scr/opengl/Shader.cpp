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
    return "";
}

int Shader::compileShader(GLenum type, const std::string& source) {
    int id = glCreateShader(type);
    glShaderSource(id, 1, reinterpret_cast<const GLchar *const *>(source.c_str()), nullptr);
    glCompileShader(id);

    return id;
}

void Shader::bind() const {
    glUseProgram(_id);
}

void Shader::unbind() {
    glUseProgram(0);
}


