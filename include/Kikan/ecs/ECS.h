#ifndef KIKAN_ECS_H
#define KIKAN_ECS_H

#include <vector>
#include "Kikan/ecs/Scene.h"

namespace Kikan { namespace Ecs {
    class ECS {
    public:
        ECS(const std::string& name = "default");
        ~ECS();

        Scene* getScene(const std::string& = "default");
        void addScene(const std::string& name);
        void setCurrScene(const std::string& name = "default");

    private:

        std::vector<Scene*> _scenes;
        Scene* _currScene;
    };
}}

#endif //KIKAN_ECS_H
