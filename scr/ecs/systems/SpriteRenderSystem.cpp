#include "SpriteRenderSystem.h"

#include "../../Engine.h"
#include "../components/TriangleSprite.h"
#include "../components/PolygonSprite.h"

SpriteRenderSystem::SpriteRenderSystem() {
    singleInclude(TriangleSprite);
    singleInclude(PolygonSprite);
}

void SpriteRenderSystem::update(double dt) {
    for (Entity* e : _entities) {
        auto* triangleSprite = e->getComponent<TriangleSprite>();
        if(triangleSprite) {
            Engine::get()->getRenderer()->renderTriangle(
                    triangleSprite->points[0],
                    triangleSprite->points[1],
                    triangleSprite->points[2],
                    triangleSprite->color,
                    triangleSprite->layer);
        }

        auto* polygonSprite = e->getComponent<PolygonSprite>();
        if(polygonSprite){
            Engine::get()->getRenderer()->renderPolygon(
                    polygonSprite->points,
                    polygonSprite->color,
                    polygonSprite->layer);
        }
    }
}

