#include "Kikan/opengl/buffers/Texture2D.h"

namespace Kikan {

    Texture2D::Texture2D(GLsizei width, GLsizei height, float data[]) : _width(width), _height(height){
        glGenTextures(1, &_id);
        bind(0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_FLOAT, data);

        unbind();
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &_id);
    }

    void Texture2D::bind(GLuint slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, _id);
    }

    void Texture2D::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint Texture2D::get() const {
        return _id;
    }
}