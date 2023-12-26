#ifndef KIKAN_AASPRITE_H
#define KIKAN_AASPRITE_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "IComponent.h"

namespace Kikan {
    // Axis Aligned Texture 2D sprite
    struct AASprite : IComponent {
    public:
        glm::vec2 offset;
        glm::vec2 dimensions;
        glm::vec4 color;
        GLuint textureID;
        float layer;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_AASPRITE_H
