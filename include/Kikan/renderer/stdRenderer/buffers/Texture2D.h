#ifndef KIKAN_TEXTURE2D_H
#define KIKAN_TEXTURE2D_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Kikan {
    class Texture2D {
    public:
        struct Options {
            GLint level = 0;
            GLint internalformat = GL_RGBA;
            GLenum format = GL_RGBA;
            GLenum type = GL_UNSIGNED_BYTE;

            GLint minFilter = GL_NEAREST;
            GLint maxFilter = GL_NEAREST;
            GLint warpS = GL_CLAMP_TO_EDGE;
            GLint warpT = GL_CLAMP_TO_EDGE;
        };

        Texture2D(GLsizei width, GLsizei height, uint8_t* data, Options* opt = nullptr);
        ~Texture2D();

        GLuint get() const;

        void bind(GLuint slot = 0) const;
        static void unbind();
    private:
        GLuint _id;
        GLsizei _width;
        GLsizei _height;
    };
}

#endif //KIKAN_TEXTURE2D_H
