#ifndef KIKAN_AUTO_BATCH_H
#define KIKAN_AUTO_BATCH_H

#include "Batch.h"

class AutoBatch{
public:
     explicit AutoBatch(VertexBufferLayout* vbl, GLuint vertexSize);
    ~AutoBatch();

    int addVertices(std::vector<IVertex*>& vertices);

    void bind();
    void render();

private:

    struct VertexSpace {
        char* data = nullptr;
        unsigned int count = 0;
        GLuint size = 0;
    };

    int _max_texture_units = 4;
    float* _texture_slots;
    int _last_slot = 0;
    int find_texture(float texture);

    VertexBufferLayout* _vbl;
    VertexSpace _vertex_space;
};


#endif //KIKAN_AUTO_BATCH_H
