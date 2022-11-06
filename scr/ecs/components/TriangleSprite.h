#ifndef KIKAN_TRIANGLE_SPRITE_H
#define KIKAN_TRIANGLE_SPRITE_H

#include "glm/glm.hpp"
#include "IComponent.h"

struct TriangleSprite : IComponent{
    glm::vec3 points[3];
    glm::vec4 color;
};

#endif //KIKAN_TRIANGLE_SPRITE_H
