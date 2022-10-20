#include "Engine.h"

#include "ecs/systems/PhysicsSystem.h"
#include "ecs/Entity.h"

int WinMain() {
    Engine engine;

    engine.getScene()->addSystem(new PhysicsSystem());
    Entity* entity = new Entity();
    entity->addComponent(new Physics);

    engine.getScene()->addEntity(entity);

    while (engine.shouldRun()) {
        engine.update(0);
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif
