#ifndef KIKAN_IVERTEX_H
#define KIKAN_IVERTEX_H

#include "GL/glew.h"
#include "../Buffer/VertexBufferLayout.h"

struct IVertex {
public:
    /*
     * When auto-batching you set the textureSlot as the texture ID.
     * That ID gets translated into a textureSlot
     */
    int texture{};

    virtual VertexBufferLayout* getLayout() {
        return nullptr;
    };
};

#endif //KIKAN_IVERTEX_H
