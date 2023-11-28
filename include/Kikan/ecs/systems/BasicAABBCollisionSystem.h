#ifndef KIKAN_BASICAABBCOLLISIONSYSTEM_H
#define KIKAN_BASICAABBCOLLISIONSYSTEM_H

#include "Kikan/ecs/systems/ISystem.h"

namespace Kikan {
    class BasicAABBCollisionSystem : public ISystem {
        BasicAABBCollisionSystem();

        void update(double dt) override;
    };

}

#endif //KIKAN_BASICAABBCOLLISIONSYSTEM_H
