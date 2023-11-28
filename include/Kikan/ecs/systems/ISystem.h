#ifndef KIKAN_ISYSTEM_H
#define KIKAN_ISYSTEM_H

#include <algorithm>
#include "vector"
#include "Kikan/ecs/TypeRegistry.h"
#include "Kikan/ecs/Entity.h"

namespace Kikan {
    class ISystem{
    public:
        ISystem() = default;

        virtual ~ISystem(){
            for (auto signatures : _signatures) {
                delete signatures;
            }
        }

        virtual void update(double dt) = 0;

        virtual void addEntity(Entity* entity){
            _entities.push_back(entity);
        }

        virtual void removeEntity(Entity* entity){
            for (int i = 0; i < (int)_entities.size(); ++i) {
                if(_entities.at(i) == entity){
                    _entities.erase(_entities.begin() + i);
                    return;
                }
            }
        }

        std::vector<std::vector<unsigned int>*>& getSignatures(){
            return _signatures;
        }

    protected:
        void includeSignatures(std::vector<unsigned int>* signatures){
            std::sort(signatures->begin(), signatures->end());
            _signatures.push_back(signatures);
        }

        std::vector<Entity*> _entities;

    private:
        std::vector<std::vector<unsigned int>*> _signatures;
    };
}
#endif //KIKAN_ISYSTEM_H
