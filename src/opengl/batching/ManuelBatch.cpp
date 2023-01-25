#include <iostream>
#include <cstring>
#include "Kikan/opengl/batching/ManuelBatch.h"
#include "Kikan/opengl/buffers/VertexBuffer.h"
#include "Kikan/opengl/buffers/IndexBuffer.h"

namespace Kikan {
    ManuelBatch::ManuelBatch(VertexBufferLayout *vbl, GLuint vertexSize) :
     _vbl(vbl) {
        _vertex_space.size = vertexSize;
    }

    ManuelBatch::~ManuelBatch() {
        delete _vertex_space.data;
    }

    template<class T>
    void ManuelBatch::updateVertices(std::vector<T> &vertices, std::vector<GLuint> &indices, unsigned int begin, unsigned int end) {
        if(sizeof(T) != _vertex_space.size){
            std::cout << "[MANUEL BATCH] Cannot add vertices of different size to batch" << std::endl;
            return;
        }

        //allocate enough space for all new vertices
        _vertex_space.count -= (end - begin) + vertices.size();

        char *data = static_cast<char *>(malloc(_vertex_space.size * _vertex_space.count));
        if(data == nullptr){
            std::cout << "[MANUEL BATCH] Error allocating memory" << std::endl;
            return;
        }

        //handle old data
        //copy before updated segment
        memcpy(data, _vertex_space.data, _vertex_space.size * begin);
        //copy after updated segment
        if(_vertex_space.size - end > 0)
            memcpy(data + (begin + vertices.size()) * _vertex_space.size, _vertex_space.data + end * _vertex_space.size, _vertex_space.size - end);
        free(_vertex_space.data);

        //copy new data
        _vertex_space.data = memcpy(data + begin * _vertex_space.size, vertices, _vertex_space.size);
    }

    template<class T>
    void ManuelBatch::addVertices(std::vector<T> &vertices, std::vector<GLuint> &indices) {
        if(sizeof(T) != _vertex_space.size){
            std::cout << "[MANUEL BATCH] Cannot add vertices of different size to batch" << std::endl;
            return;
        }

        //allocate enough space for all new vertices
        char *data = static_cast<char *>(malloc(_vertex_space.size * (_vertex_space.count + vertices.size())));
        if(data == nullptr){
            std::cout << "[MANUEL BATCH] Error allocating memory" << std::endl;
            return;
        }

        //handle old data
        memcpy(data, _vertex_space.data, _vertex_space.size * _vertex_space.count);
        free(_vertex_space.data);

        //copy new data
        memcpy(data + _vertex_space.count * _vertex_space.size, vertices, _vertex_space.size);

        _vertex_space.count += vertices.size();
        _vertex_space.data = data;
    }

    void ManuelBatch::render() {
        //Vertex buffers
        VertexBuffer vb(_vbl, _vertex_space.size);
        vb.addVertices(_vertex_space.data, _vertex_space.count);
        vb.bind();

        //Index buffers
        IndexBuffer ib(_indices);
        ib.bind();

        //bind all textures
        for (auto t : _textures) {
            glActiveTexture(GL_TEXTURE0 + t.second);
            glBindTexture(GL_TEXTURE_2D, t.first);
        }

        //draw
        glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    void ManuelBatch::addTexture(GLsizei id, int slot) {
        _textures[id] = slot;
    }

    void ManuelBatch::removeTexture(GLsizei id) {
        _textures.erase(id);
    }
}