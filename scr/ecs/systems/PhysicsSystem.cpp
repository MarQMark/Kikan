#include "PhysicsSystem.h"
#include "../components/Physics.h"

namespace Kikan {
    PhysicsSystem::PhysicsSystem() {
        singleInclude(Physics);
    }

    void PhysicsSystem::update(double dt) {
    }
}

