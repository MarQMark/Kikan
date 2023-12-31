#ifndef KIKAN_ECSTHREAD_H
#define KIKAN_ECSTHREAD_H

#include <thread>
#include "Kikan/ecs/systems/ISystem.h"
#include "Kikan/ecs/Entity.h"

namespace Kikan {
    class EcsThread {
        enum Status {
            UNKNOWN,
            RUNNING,
            STOPPED,
        };

    public:
        EcsThread(uint32_t cycleTime, uint32_t timout);
        ~EcsThread();

        void start();
        void stop();

        Status getStatus();

        void addEntity(Entity* entity);
        void removeEntity(Entity* entity);
        void addSystem(ISystem* system);

    private:
        void run();
        void update();

        std::thread* _t;

        std::vector<Entity*> _entities;
        std::vector<ISystem*> _systems;

        Status _status = UNKNOWN;
        bool _shouldRun = true;

        uint32_t _cycleTime;
        uint32_t _timout;
        uint32_t _maxTimeout;
    };
}

#endif //KIKAN_ECSTHREAD_H
