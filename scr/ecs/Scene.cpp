#include "Scene.h"

std::string Scene::getName() {
    return _name;
}

void Scene::addEntity(Entity* entity) {
    _entities.push_back(entity);
}

void Scene::update(double dt) {

}
