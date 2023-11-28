#ifndef KIKAN_UTIL_H
#define KIKAN_UTIL_H

#include <vector>
#include "Kikan/ecs/Entity.h"
#include "Kikan/ecs/systems/ISystem.h"

namespace Kikan { namespace Ecs {

    class Util{
    public:
        static bool isSubset(std::vector<unsigned int>& systemSignature, std::vector<unsigned int>& entitySignature) {
            return std::includes(entitySignature.begin(), entitySignature.end(), systemSignature.begin(),
                                 systemSignature.end());
        }

        static void addEntity(Entity *entity, std::vector<Entity*>* entities, std::vector<ISystem*>* systems) {
            entities->push_back(entity);

            std::vector<unsigned int> entitySignature = entity->getSignatures();
            std::sort(entitySignature.begin(), entitySignature.end());

            for (ISystem *system: *systems) {
                bool containsAll = false;

                for (auto systemSignature: system->getSignatures()) {
                    if (isSubset(*systemSignature, entitySignature)) {
                        containsAll = true;
                        break;
                    }
                }

                if (containsAll)
                    system->addEntity(entity);
            }
        }

        static void removeEntity(Entity *entity, std::vector<Entity*>* entities, std::vector<ISystem*>* systems) {
            std::vector<unsigned int> entitySignature = entity->getSignatures();
            std::sort(entitySignature.begin(), entitySignature.end());

            for (ISystem *system: *systems) {
                bool containsAll = false;

                for (auto systemSignature: system->getSignatures()) {
                    if (isSubset(*systemSignature, entitySignature)) {
                        containsAll = true;
                        break;
                    }
                }

                if (containsAll)
                    system->removeEntity(entity);
            }

            for (int i = 0; i < (int)entities->size(); ++i) {
                if (entities->at(i) == entity) {
                    entities->erase(entities->begin() + i);
                    return;
                }
            }
        }

        static void getEntities(std::vector<unsigned int>* signatures, std::vector<Entity*>* entities, std::vector<Entity*>& entitiesList){
            std::sort(signatures->begin(), signatures->end());

            for(auto* entity : entitiesList){

                std::vector<unsigned int> entitySignature = entity->getSignatures();
                std::sort(entitySignature.begin(), entitySignature.end());

                if (isSubset(*signatures, entitySignature))
                    entities->push_back(entity);
            }

            delete signatures;
        }

        static Entity* getEntity(std::vector<unsigned int>* signatures, std::vector<Entity*>& entitiesList) {
            Kikan::Entity* e = nullptr;

            std::sort(signatures->begin(), signatures->end());

            for(auto* entity : entitiesList){

                std::vector<unsigned int> entitySignature = entity->getSignatures();
                std::sort(entitySignature.begin(), entitySignature.end());

                if (isSubset(*signatures, entitySignature)){
                    e =  entity;
                    break;
                }
            }

            delete signatures;
            return e;
        }
    };
}}

#endif //KIKAN_UTIL_H
