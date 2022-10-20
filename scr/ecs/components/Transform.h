#ifndef KIKAN_TRANSFORM_H
#define KIKAN_TRANSFORM_H

#include "glm/glm.hpp"
#include "IComponent.h"

struct Transform : IComponent {
    glm::vec2 _position = glm::vec2(0.0, 0.0);
    glm::vec2 _scale = glm::vec2(1.0, 1.0);;
    float _rotation = 0.0;
};

#endif //KIKAN_TRANSFORM_H
