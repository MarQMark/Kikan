#include "Engine.h"

#include "ecs/systems/PhysicsSystem.h"
#include "ecs/Entity.h"

int WinMain() {
    Engine engine;

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
