#ifndef KIKAN_BATCH_H
#define KIKAN_BATCH_H

#include "VertexBuffer.h"

class Batch {
public:

    void addVertices();
    void updateVertices();
    void removeVertices();

    void bind();
    void unbind();

    void render();
private:
    GLsizei _last_vertex = 0;
};


#endif //KIKAN_BATCH_H
