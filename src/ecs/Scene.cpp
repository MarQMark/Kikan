#include "Kikan/ecs/Scene.h"
#include "Kikan/ecs/systems/IRenderSystem.h"
#include "Kikan/core/Logging.h"
#include "Kikan/ecs/Util.h"

namespace Kikan { namespace Ecs {
    std::string Scene::name() {
        return _name;
    }

    void Scene::addEntity(Entity *entity) {
        Util::addEntity(entity, &_entities, &_systems);
        for(auto* thread : _threads)
            thread->addEntity(entity);
    }

    void Scene::removeEntity(Entity *entity) {
        Util::removeEntity(entity, &_entities, &_systems);
    }

    void Scene::deleteEntity(Entity *entity) {
        for(auto* thread : _threads)
            thread->removeEntity(entity);
        removeEntity(entity);
        delete entity;
    }

    void Scene::addSystem(ISystem *system) {
        _systems.push_back(system);
    }

    void Scene::update(double dt) {
        for (ISystem *system: _systems)
            system->update(dt);
    }

    void Scene::createThread(uint32_t cycle, uint32_t timeout) {
        auto* thread = new EcsThread(cycle, timeout);
        thread->start();
        _threads.push_back(thread);
    }

    void Scene::addThreadedSystem(ISystem *system, uint32_t id) {
        if (_threads.size() - 1 < id){
            kikanPrintE("No ECS thread with ID: %d exists\n", id);
            return;
        }

        _threads.at(id)->addSystem(system);
    }

    void Scene::removeThreadedEntity(Entity *entity, uint32_t id) {
        if (_threads.size() - 1 < id){
            kikanPrintE("No ECS thread with ID: %d exists\n", id);
            return;
        }

        _threads.at(id)->removeEntity(entity);
    }

    Entity *Scene::getEntity(std::vector<unsigned int>* signatures) {
        return Util::getEntity(signatures, _entities);
    }

    void Scene::getEntities(std::vector<unsigned int> *signatures, std::vector<Entity *>* entities) {
        Util::getEntities(signatures, entities, _entities);
    }
} }