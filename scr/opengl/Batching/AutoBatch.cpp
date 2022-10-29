#include "AutoBatch.h"
#include <algorithm>
#include <iostream>

AutoBatch::AutoBatch(VertexBufferLayout* vbl, GLuint vertexSize) {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_max_texture_units);
    _texture_slots = new GLuint[_max_texture_units];
    _vertex_size = vertexSize;
    _vbl = vbl;
}


AutoBatch::~AutoBatch() {
    delete _texture_slots;
}

int AutoBatch::find_texture(GLuint texture){
    for (int i = 0; i < _max_texture_units; ++i) {
        if(_texture_slots[i] == texture)
            return i;
    }

    return -1;
}

int AutoBatch::addVertices(std::vector<std::shared_ptr<IVertex>>& vertices) {
    // Add all textures;
    int i;
    for (i = 0; i < vertices.size(); i++) {
        if(vertices[i]->texture >= 0){
            int pos = find_texture(vertices[i]->texture);

            //texture already there
            if(pos != -1){
                vertices[i]->texture = pos;
            } else{
                // no more space in Batch
                if(!_texture_slots[_max_texture_units - 1]){
                    break;
                } else{
                    _texture_slots[_last_slot] = vertices[i]->texture;
                    vertices[i]->texture = _last_slot;
                    _last_slot++;
                }
            }
        }
    }

    _vertices.reserve(_vertices.size() + i);
    for (int j = 0; j < i; j++){
        _vertices.push_back(vertices[j]);
    }

    return i == vertices.size() ? 0 : i;
}

void AutoBatch::bind() {
    for (int i = 0; i < _last_slot; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, _texture_slots[i]);
    }
}

void AutoBatch::render() {
    VertexBuffer vb(_vbl, _vertex_size);
    vb.bind();
    vb.addVertices(_vertices);

    bind();

    _last_vertex = (GLsizei)_vertices.size();

    Batch::render();
}