#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(GLsizei size) {
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}


void VertexBuffer::addVertices(std::vector<IVertex>& vertices) {

}

void VertexBuffer::bind() {
    //load layout

    //glEnableVertexAttribArray
    //glVertexAttribPointer
}

