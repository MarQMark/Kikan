#ifndef KIKAN_TEXTURE2D_SPRITE_H
#define KIKAN_TEXTURE2D_SPRITE_H

#include "glm/glm.hpp"
#include "GL/glew.h"
#include "IComponent.h"

namespace Kikan {
    struct Texture2DSprite : IComponent {
    public:
        glm::vec2 points[4];
        glm::vec2 texCoords[4] = {
                glm::vec2 (0, 1),
                glm::vec2 (1, 1),
                glm::vec2 (1, 0),
                glm::vec2 (0, 0),
        };
        glm::vec4 color;
        GLuint textureID;
        float layer;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_TEXTURE2D_SPRITE_H
