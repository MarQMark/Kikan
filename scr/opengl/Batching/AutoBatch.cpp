#include "AutoBatch.h"
#include <algorithm>
#include <iostream>

AutoBatch::AutoBatch(VertexBufferLayout* vbl, GLuint vertexSize) {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_max_texture_units);
    _texture_slots = new float[_max_texture_units];
    _vertex_space.size = vertexSize;
    _vbl = vbl;
}


AutoBatch::~AutoBatch() {
    delete _texture_slots;
    free(_vertex_space.data);
}

int AutoBatch::find_texture(float texture){
    for (int i = 0; i < _max_texture_units; ++i) {
        if(_texture_slots[i] == texture)
            return i;
    }

    return -1;
}

int AutoBatch::addVertices(std::vector<IVertex*>& vertices) {
    // Add all textures;
    int i;
    for (i = 0; i < vertices.size(); i++) {
        if(vertices[i]->texture >= 0){
            int pos = find_texture(vertices[i]->texture);

            //texture already there
            if(pos != -1){
                vertices[i]->texture = (float)pos;
            } else{
                // no more space in Batch
                if(!(bool)_texture_slots[_max_texture_units - 1]){
                    break;
                } else{
                    _texture_slots[_last_slot] = vertices[i]->texture;
                    vertices[i]->texture = (float)_last_slot;
                    _last_slot++;
                }
            }
        }
    }

    //allocate enough space for all new vertices
    char* data = static_cast<char *>(malloc(_vertex_space.size * (_vertex_space.count + i)));

    //handle old data
    memcpy(data, _vertex_space.data, _vertex_space.size * _vertex_space.count);
    free(_vertex_space.data);

    //copy new data
    for (int j = 0; j < i; ++j)
        memcpy(data + (j + _vertex_space.count) * _vertex_space.size, vertices[j],_vertex_space.size);

    _vertex_space.count += i;
    _vertex_space.data = data;

    return i == vertices.size() ? 0 : i;
}

void AutoBatch::bind() {
    for (int i = 0; i < _last_slot; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, (unsigned int)_texture_slots[i]);
    }
}

void AutoBatch::render() {
    VertexBuffer vb(_vbl, _vertex_space.size);
    vb.bind();
    vb.addVertices(_vertex_space.data, _vertex_space.count);

    bind();

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)_vertex_space.size);
}