#include "Scene.h"

std::string Scene::getName() {
    return _name;
}

bool isSubset(std::vector<unsigned int> &systemSignature, std::vector<unsigned int> &entitySignature){
    return std::includes(entitySignature.begin(), entitySignature.end(), systemSignature.begin(), systemSignature.end());
}

void Scene::addEntity(Entity* entity) {
    _entities.push_back(entity);

    std::vector<unsigned int> entitySignature = entity->getSignatures();
    std::sort(entitySignature.begin(), entitySignature.end());

    for (ISystem *system : _systems) {
        bool containsAll = false;

        for(auto systemSignature : system->getSignatures()){
            if(isSubset(*systemSignature, entitySignature)){
                containsAll = true;
                break;
            }
        }

        if(containsAll)
            system->addEntity(entity);
    }
}

void Scene::removeEntity(Entity *entity) {
    std::vector<unsigned int> entitySignature = entity->getSignatures();
    std::sort(entitySignature.begin(), entitySignature.end());

    for (ISystem *system : _systems) {
        bool containsAll = false;

        for(auto systemSignature : system->getSignatures()){
            if(isSubset(*systemSignature, entitySignature)){
                containsAll = true;
                break;
            }
        }

        if(containsAll)
            system->removeEntity(entity);
    }

    for (int i = 0; i < _entities.size(); ++i) {
        if(_entities.at(i) == entity){
            _entities.erase(_entities.begin() + i);
            return;
        }
    }
}

void Scene::deleteEntity(Entity* entity){
    removeEntity(entity);
    delete entity;
}

void Scene::addSystem(ISystem *system) {
    _systems.push_back(system);
}

void Scene::update(double dt) {
    for(ISystem* system : _systems)
        system->update(dt);
}
