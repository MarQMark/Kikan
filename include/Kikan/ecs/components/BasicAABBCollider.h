#ifndef KIKAN_BASICAABBCOLLIDER_H
#define KIKAN_BASICAABBCOLLIDER_H

#include "Kikan/ecs/components/IComponent.h"
#include "glm/vec2.hpp"

namespace Kikan {
    struct BasicAABBCollider : IComponent {
    public:
        /* Position is specified by transform and defines top left corner */

        /* Width, Height */
        glm::vec2 dimensions;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_BASICAABBCOLLIDER_H
