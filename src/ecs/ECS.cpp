#include "Kikan/ecs/ECS.h"
#include "Kikan/core/Logging.h"
#include "Kikan/ecs/Util.h"

namespace Kikan {
    ECS::ECS(void* params) {
        auto initParams = (struct InitParams*)params;

        bool defaultParams = false;
        if(!initParams) {
            defaultParams = true;
            initParams = new InitParams;
        }

        auto* defaultScene = new Scene(initParams->name);
        _currScene = defaultScene;
        addScene(defaultScene);
        _scenes["default"] = defaultScene;

        if(defaultParams)
            delete initParams;
    }

    ECS::~ECS() {
        for (auto* thread : _threads){
            thread->stop();
            delete thread;
        }
        for (auto p : _scenes) {
            delete p.second;
        }
        _scenes.clear();
    }

    Scene *ECS::getScene(const std::string& name) {
        if(_scenes.count(name))
            return _scenes[name];

        //return default if no scene found
        return _scenes["default"];
    }

    void ECS::addScene(Scene* scene) {
        if(_scenes.count(scene->name())){
            kikanPrintE("[ERROR] Ecs: Scene %s already exists\n", scene->name().c_str());
            return;
        }

        _scenes[scene->name()] = scene;
    }

    void ECS::removeScene(const std::string &name) {
        if(_scenes.count(name))
            _scenes.erase(name);
    }

    void ECS::deleteScene(const std::string &name) {
        if(_scenes.count(name))
            delete _scenes[name];
        removeScene(name);
    }

    void ECS::loadScene(const std::string &name) {
        if(!_scenes.count(name)){
            kikanPrintE("[ERROR] Ecs: Scene %s does not exist\n", name.c_str());
            return;
        }

        _currScene->unload();
        _currScene = _scenes[name];
        _currScene->load();
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
        for(auto p : _scenes){
            p.second->removeEntity(entity);
        }
        for(auto* thread : _threads)
            thread->removeEntity(entity);
        delete entity;
    }

    Entity *ECS::getEntity(std::vector<unsigned int> *signatures) {
        return Util::getEntity(signatures, _entities);
    }

    void ECS::getEntities(std::vector<unsigned int> *signatures, std::vector<Entity *> *entities) {
        Util::getEntities(signatures, entities, _entities);
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

    void ECS::update(double dt) {
        for (ISystem *system: _systems)
            system->update(dt);

        _currScene->update(dt);
    }
}
