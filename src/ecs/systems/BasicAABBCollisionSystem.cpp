#include "Kikan/ecs/systems/BasicAABBCollisionSystem.h"
#include "Kikan/ecs/components/BasicAABBCollider.h"

Kikan::BasicAABBCollisionSystem::BasicAABBCollisionSystem() {
    singleInclude(BasicAABBCollider);
}

void Kikan::BasicAABBCollisionSystem::update(double dt) {

}
