#ifndef KIKAN_AASPRITE_H
#define KIKAN_AASPRITE_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "IComponent.h"

namespace Kikan {
    // Axis Aligned Texture 2D sprite
    struct AASprite : IComponent {
    public:
        glm::vec2 offset = glm::vec2(0);
        glm::vec2 dimensions;
        glm::vec4 color;
        GLuint textureID;
        glm::vec2 texCoords[4] = {
                glm::vec2 (0, 1),
                glm::vec2 (1, 1),
                glm::vec2 (1, 0),
                glm::vec2 (0, 0),
        };
        float layer = 0;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_AASPRITE_H
