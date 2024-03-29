#ifndef KIKAN_ENTITY_H
#define KIKAN_ENTITY_H

#include "vector"
#include "map"
#include "string"
#include "typeinfo"
#include "TypeRegistry.h"
#include "Kikan/ecs/components/IComponent.h"
#include "Kikan/ecs/components/Transform.h"

namespace Kikan {
    class Entity {
    public:
        Entity(){
            Transform* transform = new Transform();
            addComponent<Transform>(transform);
        }

        ~Entity(){
            for(auto & component : _components)
                component.second->destroy();
        }

        template<class T>
        void addComponent(T* component){
            unsigned int signature = TypeRegistry::getSignature<T>();
            _components[signature] = component;

            for(unsigned int sig : _signatures){
                if(sig == signature)
                    return;
            }

            _signatures.push_back(signature);
        }

        template<class T>
        T* getComponent(){
            unsigned int sig = TypeRegistry::getSignature<T>();
            if(_components.size() > 0 && _components.count(sig))
                return (T*)_components[TypeRegistry::getSignature<T>()];

            return nullptr;
        }

        std::vector<unsigned int> getSignatures() const{
            return _signatures;
        }

    private:
        std::map<unsigned int, IComponent*> _components;
        std::vector<unsigned int> _signatures;
    };
}

#endif //KIKAN_ENTITY_H
