#ifndef KIKAN_PHYSICS_SYSTEM_H
#define KIKAN_PHYSICS_SYSTEM_H

#include "ISystem.h"

namespace Kikan {
    class PhysicsSystem : public ISystem {
    public:
        PhysicsSystem();

        void update(double dt) override;

        float gravity = 10;
        float friction = 0.1f;
    };
}

#endif //KIKAN_PHYSICS_SYSTEM_H
