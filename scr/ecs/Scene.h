#ifndef KIKAN_SCENE_H
#define KIKAN_SCENE_H

#include <utility>
#include <vector>
#include "string"
#include "Entity.h"
#include "systems/ISystem.h"

class Scene {
public:
    Scene(std::string name)
        : _name(std::move(name)){
    }

    ~Scene(){
        for(Entity* entity : _entities)
            delete entity;

        for(ISystem* system : _systems)
            delete system;
    }

    std::string getName();

    void update(double dt);

    void addEntity(Entity* entity);
    void addSystem(ISystem* entity);

private:
    const std::string _name;

    std::vector<Entity*> _entities;
    std::vector<ISystem*> _systems;
};


#endif //KIKAN_SCENE_H
