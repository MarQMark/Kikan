#ifndef KIKAN_ENTITY_H
#define KIKAN_ENTITY_H

#include "vector"
#include "components/IComponent.h"
#include "components/Transform.h"

class Entity {
public:
    Entity(){
        Transform* transform = new Transform();
        _components.push_back(transform);
    }

    ~Entity(){
        for(IComponent* component : _components)
            delete component;
    }

private:
    std::vector<IComponent*> _components;
};


#endif //KIKAN_ENTITY_H
