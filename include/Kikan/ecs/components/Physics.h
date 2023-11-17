#ifndef KIKAN_PHYSICS_H
#define KIKAN_PHYSICS_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct Physics : IComponent {
    public:
        glm::vec2 velocity;
        glm::vec2 acceleration;

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_PHYSICS_H
