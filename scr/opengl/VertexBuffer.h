#ifndef KIKAN_VERTEX_BUFFER_H
#define KIKAN_VERTEX_BUFFER_H

#include "GL/glew.h"
#include <GLFW/glfw3.h>

class VertexBuffer {
public:
    explicit VertexBuffer(GLsizei size);

private:
    GLuint _id = 0;
};


#endif //KIKAN_VERTEX_BUFFER_H
