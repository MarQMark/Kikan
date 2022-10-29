#include <iostream>
#include "VertexBuffer.h"
#include "../Vertices/DefaultVertex.h"

VertexBuffer::VertexBuffer(VertexBufferLayout* vbl, unsigned int vertexSize) {
    _vbl = vbl;
    _vertex_size = vertexSize;
    glGenBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &_id);
}

void VertexBuffer::addVertices(std::vector<std::shared_ptr<IVertex>>& vertices) const {
    unsigned int size = _vertex_size * vertices.size();

    char* data = static_cast<char *>(malloc(size));
    for (int i = 0; i < vertices.size(); ++i)
        memcpy((data + i * _vertex_size), vertices[i].get(), _vertex_size);

    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    free(data);
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, _id);

    //load layout
    const std::vector<VertexBufferElement>& elements = _vbl->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); ++i) {
        const VertexBufferElement& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized,
                              _vbl->getStride(), (const void *)(size_t)offset);
        offset += element.count * VertexBufferElement::getTypeSize(element.type);
    }
}