#include "Kikan/ecs/ECS.h"
#include "Kikan/core/Logging.h"
#include "Kikan/ecs/Util.h"

namespace Kikan { namespace Ecs {
    ECS::ECS(const std::string& name) {
        auto* defaultScene = new Scene(name);
        _currScene = defaultScene;
        _scenes.push_back(defaultScene);
    }

    ECS::~ECS() {
        for (auto* thread : _threads){
            thread->stop();
            delete thread;
        }
        for (Scene* scene : _scenes) {
            delete scene;
        }
        _scenes.clear();
    }

    Scene *ECS::getScene(const std::string& name) {
        //look for specified scene
        for (Scene* scene : _scenes) {
            if (scene->name() == name)
                return scene;
        }

        //return default if no scene found
        return _scenes.at(0);
    }

    void ECS::addScene(const std::string& name) {
        auto* scene = new Scene(name);
        _scenes.push_back(scene);
    }

    void ECS::setCurrScene(const std::string& name) {
        _currScene = getScene(name);
    }

    void ECS::addEntity(Entity *entity) {
        Util::addEntity(entity, &_entities, &_systems);
        for(auto* thread : _threads)
            thread->addEntity(entity);
    }

    void ECS::removeEntity(Entity *entity) {
        Util::removeEntity(entity, &_entities, &_systems);
    }

    void ECS::deleteEntity(Entity *entity) {
        removeEntity(entity);
        for(auto* scene : _scenes){
            scene->removeEntity(entity);
        }
        for(auto* thread : _threads)
            thread->removeEntity(entity);
        delete entity;
    }

    void ECS::addSystem(ISystem *system) {
        _systems.push_back(system);
    }

    void ECS::createThread(uint32_t cycle, uint32_t timeout) {
        auto* thread = new EcsThread(cycle, timeout);
        thread->start();
        _threads.push_back(thread);
    }

    void ECS::addThreadedSystem(ISystem *system, uint32_t id) {
        if (_threads.size() - 1 < id){
            kikanPrintE("No ECS thread with ID: %d exists\n", id);
            return;
        }

        _threads.at(id)->addSystem(system);
    }

    void ECS::removeThreadedEntity(Entity *entity, uint32_t id) {
        if (_threads.size() - 1 < id){
            kikanPrintE("No ECS thread with ID: %d exists\n", id);
            return;
        }

        _threads.at(id)->removeEntity(entity);
    }

}}
