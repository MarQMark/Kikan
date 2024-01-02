#include <algorithm>
#include <iostream>
#include <cstring>
#include "Kikan/renderer/stdRenderer/batching/AutoBatch.h"
#include "Kikan/renderer/stdRenderer/buffers/IndexBuffer.h"
#include "Kikan/Engine.h"

namespace Kikan {
    void AutoBatchPreRender(AutoBatch* batch, void* data){
        auto* shader = (Shader*)data;
        shader->bind();
        shader->uniformM4fv("u_mvp", ((StdRenderer*)Engine::Kikan()->getRenderer())->mvp);
    }

    AutoBatch::AutoBatch(VertexBufferLayout *vbl, GLuint vertexSize, float textureID) : _textureID(textureID) {
        _vertex_space.size = vertexSize;
        _vbl = vbl;

        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        _pre_render_data = renderer->shader();
        _pre_render = AutoBatchPreRender;
    }

    AutoBatch::~AutoBatch() {
        free(_vertex_space.data);
    }

    int AutoBatch::addVertices(std::vector<IVertex *> &vertices, int start, int stop) {
        int n = add_vertices(vertices, start, stop);

        for (int i = 0; i < (n != 0 ? n : vertices.size()); ++i)
            _indices.push_back(_next_index++);

        return n;
    }

    int AutoBatch::addVertices(std::vector<IVertex *> &vertices, std::vector<GLuint> &indices, int start, int stop) {
        int n = add_vertices(vertices, start, stop);

        //return error
        if (n == -1)
            return -1;

        //get max index from new indices
        GLuint maxIndex = 0;
        for (GLuint index: indices) {
            maxIndex = std::max(maxIndex, index);
            _indices.push_back(_next_index + index);
        }
        _next_index += maxIndex + 1;

        return 0;
    }

    int AutoBatch::add_vertices(std::vector<IVertex *> &vertices, int start, int stop) {
        int count = stop - start + 1;

        //allocate enough space for all new vertices
        char *data = static_cast<char *>(malloc(_vertex_space.size * (_vertex_space.count + count)));
        if(data == nullptr)
            return -1;

        //handle old data
        memcpy(data, _vertex_space.data, _vertex_space.size * _vertex_space.count);
        free(_vertex_space.data);

        //copy new data
        for (int i = 0; i < count; ++i)
            memcpy(data + (i + _vertex_space.count) * _vertex_space.size, vertices[i], _vertex_space.size);

        _vertex_space.count += count;
        _vertex_space.data = data;

        return 0;
    }

    void AutoBatch::render() {
        //Vertex buffers
        VertexBuffer vb(_vbl, _vertex_space.size);
        vb.addVertices(_vertex_space.data, _vertex_space.count);
        vb.bind();

        //Index buffers
        IndexBuffer ib(_indices);
        ib.bind();

        //bind all textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (unsigned int) _textureID);

        if(_pre_render)
            _pre_render(this, _pre_render_data);

        //draw
        glDrawElements(GL_TRIANGLES, (GLsizei) _indices.size(), GL_UNSIGNED_INT, nullptr);

        if(_post_render)
            _post_render(this, _post_render_data);
    }

    void AutoBatch::addPreRender(void (*fn)(AutoBatch *, void *), void *data) {
        _pre_render = fn;
        _pre_render_data = data;
    }

    void AutoBatch::addPostRender(void (*fn)(AutoBatch *, void *), void *data) {
        _post_render = fn;
        _post_render_data = data;
    }
}