#ifndef KIKAN_POLYGON_SPRITE_H
#define KIKAN_POLYGON_SPRITE_H

#include <vector>
#include "glm/glm.hpp"
#include "IComponent.h"

struct PolygonSprite : IComponent {
    std::vector<glm::vec2> points;
    glm::vec4 color;
    int layer;
};

#endif //KIKAN_POLYGON_SPRITE_H