#ifndef KIKAN_ENTITY_H
#define KIKAN_ENTITY_H

#include "vector"
#include "string"
#include "typeinfo"
#include "TypeRegistry.h"
#include "components/IComponent.h"
#include "components/Transform.h"

class Entity {
public:
    Entity(){
        Transform* transform = new Transform();
        addComponent<Transform>(transform);
    }

    ~Entity(){
        for(IComponent* component : _components)
            delete component;
    }

    template<class T>
    void addComponent(T* component){
        _components.push_back(component);
        _signatures.push_back(TypeRegistry::getSignature<T>());

    }

    std::vector<unsigned int> getSignatures() const{
        return _signatures;
    }

private:
    std::vector<IComponent*> _components;
    std::vector<unsigned int> _signatures;
};


#endif //KIKAN_ENTITY_H
