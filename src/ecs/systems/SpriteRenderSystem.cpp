#include "Kikan/ecs/systems/SpriteRenderSystem.h"

#include "Kikan/ecs/components/TriangleSprite.h"
#include "Kikan/ecs/components/QuadSprite.h"
#include "Kikan/ecs/components/PolygonSprite.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/ecs/components/LineQuadSprite.h"

namespace Kikan {
    SpriteRenderSystem::SpriteRenderSystem() {
        includeSingle(TriangleSprite);
        includeSingle(QuadSprite);
        includeSingle(PolygonSprite);
        includeSingle(Texture2DSprite);
        includeSingle(LineQuadSprite);
    }

    void SpriteRenderSystem::update(double dt) {
        for (Entity *e: _entities) {
            auto *triangleSprite = e->getComponent<TriangleSprite>();
            if (triangleSprite) {
                _renderer->renderTriangle(
                        triangleSprite->points[0],
                        triangleSprite->points[1],
                        triangleSprite->points[2],
                        triangleSprite->color,
                        triangleSprite->layer);
            }

            auto *quadSprite = e->getComponent<QuadSprite>();
            if (quadSprite) {
                _renderer->renderQuad(
                        quadSprite->points[0],
                        quadSprite->points[1],
                        quadSprite->points[2],
                        quadSprite->points[3],
                        quadSprite->color,
                        quadSprite->layer);
            }

            auto *polygonSprite = e->getComponent<PolygonSprite>();
            if (polygonSprite) {
                _renderer->renderPolygon(
                        polygonSprite->points,
                        polygonSprite->color,
                        polygonSprite->layer);
            }

            auto *texture2DSprite = e->getComponent<Texture2DSprite>();
            if (texture2DSprite) {
                _renderer->renderTexture2D(
                        texture2DSprite->points[0],
                        texture2DSprite->points[1],
                        texture2DSprite->points[2],
                        texture2DSprite->points[3],
                        texture2DSprite->textureID,
                        texture2DSprite->color,
                        texture2DSprite->layer);
            }

            auto *lineQuadSprite = e->getComponent<LineQuadSprite>();
            if(lineQuadSprite){
                auto* transform = e->getComponent<Transform>();
                float width = lineQuadSprite->dimensions.x * transform->scale.x;
                float height = lineQuadSprite->dimensions.y * transform->scale.y;
                float thickness = lineQuadSprite->thickness;
                _renderer->renderQuad(
                        glm::vec2(transform->position),
                        glm::vec2(transform->position) + glm::vec2(width,   0),
                        glm::vec2(transform->position) + glm::vec2(width,   0) + glm::vec2(0, -thickness),
                        glm::vec2(transform->position) + glm::vec2(0,       -thickness),
                        lineQuadSprite->color,
                        lineQuadSprite->layer);
                _renderer->renderQuad(
                        glm::vec2(transform->position) + glm::vec2(width, 0)        + glm::vec2(-thickness, -thickness),
                        glm::vec2(transform->position) + glm::vec2(width, 0)        + glm::vec2(0,          -thickness),
                        glm::vec2(transform->position) + glm::vec2(width, -height)  + glm::vec2(0,          thickness),
                        glm::vec2(transform->position) + glm::vec2(width, -height)  + glm::vec2(-thickness, thickness),
                        lineQuadSprite->color,
                        lineQuadSprite->layer);
                _renderer->renderQuad(
                        glm::vec2(transform->position) + glm::vec2(0,       -height) + glm::vec2(0, thickness),
                        glm::vec2(transform->position) + glm::vec2(width,   -height) + glm::vec2(0, thickness),
                        glm::vec2(transform->position) + glm::vec2(width,   -height),
                        glm::vec2(transform->position) + glm::vec2(0,       -height),
                        lineQuadSprite->color,
                        lineQuadSprite->layer);
                _renderer->renderQuad(
                        glm::vec2(transform->position) + glm::vec2(0,           -thickness),
                        glm::vec2(transform->position) + glm::vec2(thickness,   -thickness),
                        glm::vec2(transform->position) + glm::vec2(0,           -height)    + glm::vec2(thickness, thickness),
                        glm::vec2(transform->position) + glm::vec2(0,           -height)    + glm::vec2(0,         thickness),
                        lineQuadSprite->color,
                        lineQuadSprite->layer);
            }
        }
    }
}