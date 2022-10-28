#ifndef KIKAN_VERTEX_BUFFER_H
#define KIKAN_VERTEX_BUFFER_H

#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "../Vertices/IVertex.h"

class VertexBuffer {
public:
    explicit VertexBuffer(GLsizei size);

    void addVertices(std::vector<IVertex>& vertices);

    void bind();

private:
    GLuint _id = 0;
};


#endif //KIKAN_VERTEX_BUFFER_H
