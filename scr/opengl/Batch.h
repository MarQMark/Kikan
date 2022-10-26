#ifndef KIKAN_BATCH_H
#define KIKAN_BATCH_H

#include "VertexBuffer.h"

enum BatchType{
    SQUARE,
    TRIANGLE
};

class Batch {
public:
    Batch(BatchType type) : _type(type) {}

    virtual void bind() = 0;
    static void unbind();

    virtual void render();
protected:
    GLsizei _last_vertex = 0;

    BatchType _type;
};


#endif //KIKAN_BATCH_H
