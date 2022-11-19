#include "Engine.h"

#include "ecs/systems/SpriteRenderSystem.h"
#include "ecs/components/QuadSprite.h"
#include "ecs/components/PolygonSprite.h"
#include "ecs/Entity.h"

int WinMain() {
    Engine engine;
    engine.getScene()->addSystem(new SpriteRenderSystem());

    auto* entity = new Entity();
    auto* sprite = new QuadSprite;
    sprite->points[0] = glm::vec2(-1, 1);
    sprite->points[1] = glm::vec2(1, 1);
    sprite->points[2] = glm::vec2(1, -1);
    sprite->points[3] = glm::vec2(-1, -1);
    sprite->color = glm::vec4(0.3, 0.4, 0.8, 1.0);
    sprite->layer = 0;
    entity->addComponent(sprite);

    /*auto* sprite = new PolygonSprite();
    std::vector<glm::vec2> points(6);
    points[0] = glm::vec2(-.5, .5);
    points[1] = glm::vec2(0, 1);
    points[2] = glm::vec2(.5, .5);
    points[3] = glm::vec2(.5, -.5);
    points[4] = glm::vec2(0, -1);
    points[5] = glm::vec2(-.5, -.5);
    sprite->points = points;
    sprite->color = glm::vec4(0.3, 0.4, 0.8, 1.0);
    sprite->layer = 0;
    entity->addComponent(sprite);*/
    engine.getScene()->addEntity(entity);

    while (engine.shouldRun()) {
        engine.update();
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif
