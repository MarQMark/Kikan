#include "ecs/systems/PhysicsSystem.h"
#include "ecs/components/Physics.h"

namespace Kikan {
    PhysicsSystem::PhysicsSystem() {
        singleInclude(Physics);
    }

    void PhysicsSystem::update(double dt) {
    }
}

