#include "Scene.h"

std::string Scene::getName() {
    return _name;
}

void Scene::addEntity(Entity* entity) {
    _entities.push_back(entity);
}

void Scene::addSystem(ISystem *system) {
    _systems.push_back(system);
}

void Scene::update(double dt) {
    for(ISystem* system : _systems)
        system->update(dt);
}
