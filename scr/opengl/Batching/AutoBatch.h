#ifndef KIKAN_AUTO_BATCH_H
#define KIKAN_AUTO_BATCH_H

#include "Batch.h"

class AutoBatch : Batch{
public:
     explicit AutoBatch(VertexBufferLayout* vbl, GLuint vertexSize);
    ~AutoBatch();

    int addVertices(std::vector<std::shared_ptr<IVertex>>& vertices);

    void bind() override;
    void render() override;

private:
    int _max_texture_units = 4;
    GLuint* _texture_slots;
    int _last_slot = 0;
    int find_texture(GLuint texture);

    VertexBufferLayout* _vbl;
    std::vector<std::shared_ptr<IVertex>> _vertices;
    GLuint _vertex_size;
};


#endif //KIKAN_AUTO_BATCH_H
