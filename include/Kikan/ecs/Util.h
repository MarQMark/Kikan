#ifndef KIKAN_UTIL_H
#define KIKAN_UTIL_H

#include <vector>
#include "Kikan/ecs/Entity.h"
#include "Kikan/ecs/systems/ISystem.h"

namespace Kikan { namespace Ecs {

    class Util{
    public:
        static bool isSubset(std::vector<unsigned int> &systemSignature, std::vector<unsigned int> &entitySignature) {
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
    };
}}

#endif //KIKAN_UTIL_H
