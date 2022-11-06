#include "SpriteRenderSystem.h"

#include "../components/TriangleSprite.h"
#include "../../Engine.h"

SpriteRenderSystem::SpriteRenderSystem() {
    includeANDStart
         signature<TriangleSprite>()
    includeANDEnd;
}

void SpriteRenderSystem::update(double dt) {
    for (Entity* e : _entities) {
        auto* sprite = e->getComponent<TriangleSprite>();
        Engine::get()->getRenderer()->renderTriangle(
                sprite->points[0], sprite->points[1], sprite->points[2], sprite->color);
    }
}

