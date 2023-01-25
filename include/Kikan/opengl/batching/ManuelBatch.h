#ifndef KIKAN_MANUEL_BATCH_H
#define KIKAN_MANUEL_BATCH_H

#include <map>
#include "Kikan/opengl/buffers/VertexBufferLayout.h"

namespace Kikan {
    class ManuelBatch {
    public:
        ManuelBatch(VertexBufferLayout* vbl, GLuint vertexSize);
        ~ManuelBatch();

        template<class T>
        void addVertices(std::vector<T>& vertices, std::vector<GLuint>& indices);

        template<class T>
        void updateVertices(std::vector<T>& vertices, std::vector<GLuint>& indices, unsigned int begin, unsigned int end);

        void removeVertices();

        void addTexture(GLsizei id, int slot);
        void removeTexture(GLsizei id);

        void render();

    private:
        struct VertexSpace {
            char* data = nullptr;
            unsigned int count = 0;
            GLuint size = 0;
        };

        VertexBufferLayout* _vbl;
        VertexSpace _vertex_space;

        std::map<unsigned int, int> _textures;

        GLuint _next_index = 0;
        std::vector<GLuint> _indices;
    };
}

#endif //KIKAN_MANUEL_BATCH_H
