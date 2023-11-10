#ifndef KIKAN_ECS_H
#define KIKAN_ECS_H

#include <vector>
#include "Kikan/ecs/Scene.h"

namespace Kikan { namespace Ecs {
    class ECS {
    public:
        ECS(const std::string& name = "default");
        ~ECS();

        Scene* getScene(const std::string& = "default");
        void addScene(const std::string& name);
        // TODO: Remove Scene
        void setCurrScene(const std::string& name = "default");

        void addEntity(Entity* entity);

        /*
         * This will NOT remove the entity from scenes
         * */
        void removeEntity(Entity* entity);

        /*
         * This will delete the entity from all scenes
         *
         * Warning deleting an entity is not thread safe
         * */
        void deleteEntity(Entity* entity);

        void addSystem(ISystem* system);
        // TODO: Remove System

        void createThread(uint32_t cycle, uint32_t timeout);
        void addThreadedSystem(ISystem* system, uint32_t id);
        void removeThreadedEntity(Entity* entity, uint32_t id);

    private:

        std::vector<Entity*> _entities;
        std::vector<ISystem*> _systems;

        std::vector<EcsThread*> _threads;

        std::vector<Scene*> _scenes;
        Scene* _currScene;
    };
}}

#endif //KIKAN_ECS_H
