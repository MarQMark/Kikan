#ifndef KIKAN_PHYSICS_SYSTEM_H
#define KIKAN_PHYSICS_SYSTEM_H

#include "ISystem.h"
#include "../components/Physics.h"

class PhysicsSystem : public ISystem{
public:
    PhysicsSystem(){
        includeANDStart
            signature<Physics>()
        includeANDEnd;
    }

    void update(double dt) override;
};

#endif //KIKAN_PHYSICS_SYSTEM_H
