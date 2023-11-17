#ifndef KIKAN_TRANSFORM_H
#define KIKAN_TRANSFORM_H

#include "glm/glm.hpp"
#include "IComponent.h"

namespace Kikan {
    struct Transform : IComponent {
    public:
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
        glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
        glm::vec3 rotation = glm::vec3(1.0, 1.0, 1.0);

        void destroy() override{
            delete this;
        };
    };
}

#endif //KIKAN_TRANSFORM_H
