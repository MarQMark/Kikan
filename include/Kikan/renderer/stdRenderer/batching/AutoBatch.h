#ifndef KIKAN_AUTO_BATCH_H
#define KIKAN_AUTO_BATCH_H

#include "Batch.h"

namespace Kikan {
    class AutoBatch {
    public:
         explicit AutoBatch(VertexBufferLayout* vbl, GLuint vertexSize, float textureID);
        ~AutoBatch();

        int addVertices(std::vector<IVertex*>& vertices, int start, int stop);
        int addVertices(std::vector<IVertex*>& vertices, std::vector<GLuint>& indices, int start, int stop);

        void render();

        void addPreRender(void(*fn)(AutoBatch*, void*), void* data);
        void addPostRender(void(*fn)(AutoBatch*, void*), void* data);
    private:
        int add_vertices(std::vector<IVertex*>& vertices, int start, int stop);

        struct VertexSpace {
            char* data = nullptr;
            unsigned int count = 0;
            GLuint size = 0;
        };

        VertexBufferLayout* _vbl;
        VertexSpace _vertex_space;

        GLuint _textureID;

        GLuint _next_index = 0;
        std::vector<GLuint> _indices;

        void(*_pre_render)(AutoBatch* batch, void* data) = nullptr;
        void(*_post_render)(AutoBatch* batch, void* data) = nullptr;
        void* _pre_render_data = nullptr;
        void* _post_render_data = nullptr;
    };
}

#endif //KIKAN_AUTO_BATCH_H
