#ifndef KIKAN_SCENE_H
#define KIKAN_SCENE_H

#include <utility>
#include <vector>
#include <thread>
#include "string"
#include "Kikan/ecs/Entity.h"
#include "Kikan/ecs/systems/ISystem.h"
#include "Kikan/ecs/EcsThread.h"

namespace Kikan { namespace Ecs {
    class Scene {
    public:
        Scene(std::string name) : _name(std::move(name)) {
        }

        ~Scene(){
            // TODO: Fix probably memory leak
            for(ISystem* system : _systems)
                delete system;

            for(auto* thread : _threads){
                delete thread;
            }
        }

        std::string name();

        void update(double dt);

        void addEntity(Entity* entity);
        void removeEntity(Entity* entity);
        void deleteEntity(Entity* entity);

        void addSystem(ISystem* system);

        void createThread(uint32_t cycle, uint32_t timeout);
        void addThreadedSystem(ISystem* system, uint32_t id);
        void removeThreadedEntity(Entity* entity, uint32_t id);

    private:
        const std::string _name;

        std::vector<Entity*> _entities;
        std::vector<ISystem*> _systems;

        std::vector<EcsThread*> _threads;
    };
} }

#endif //KIKAN_SCENE_H
