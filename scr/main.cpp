#include "Engine.h"

#include "ecs/systems/SpriteRenderSystem.h"
#include "ecs/components/TriangleSprite.h"
#include "ecs/Entity.h"

int WinMain() {
    Engine* engine = Engine::get();
    engine->getScene()->addSystem(new SpriteRenderSystem());

    auto* entity = new Entity();
    auto* sprite = new TriangleSprite;
    sprite->points[0] = glm::vec3(0.5, 0, 0);
    sprite->points[1] = glm::vec3(-0.5, 0, 0);
    sprite->points[2] = glm::vec3(0.0, 0.5, 0);
    sprite->color = glm::vec4(0.3, 0.4, 0.8, 1.0);
    entity->addComponent(sprite);
    engine->getScene()->addEntity(entity);

    while (engine->shouldRun()) {
        engine->update();
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif
