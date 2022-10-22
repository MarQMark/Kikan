#ifndef KIKAN_SHADER_H
#define KIKAN_SHADER_H

#include <string>
#include <map>
#include "GL/glew.h"

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void bind() const;
    static void unbind();
private:
    static std::string loadShaderSource(const std::string& path);
    static int compileShader(GLenum type, const std::string& source);

    GLuint _id;
    std::map<std::string, unsigned int> _uniforms;
};


#endif //KIKAN_SHADER_H
