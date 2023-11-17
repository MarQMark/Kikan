#ifndef KIKAN_LINEQUADSPRITE_H
#define KIKAN_LINEQUADSPRITE_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct LineQuadSprite : IComponent {
    public:
        /* Position is specified by transform and defines top left corner */

        /* Width, Height */
        glm::vec2 dimensions;
        glm::vec4 color;
        float thickness;
        float layer;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_LINEQUADSPRITE_H
