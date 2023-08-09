#include "Kikan/ecs/ECS.h"

namespace Kikan { namespace Ecs {
    ECS::ECS(const std::string& name) {
        auto* defaultScene = new Scene(name);
        _currScene = defaultScene;
        _scenes.push_back(defaultScene);
    }

    ECS::~ECS() {
        for (Scene* scene : _scenes) {
            delete scene;
        }
        _scenes.clear();
    }

    Scene *ECS::getScene(const std::string& name) {
        //look for specified scene
        for (Scene* scene : _scenes) {
            if (scene->name() == name)
                return scene;
        }

        //return default if no scene found
        return _scenes.at(0);
    }

    void ECS::addScene(const std::string& name) {
        //TODO implement function
    }

    void ECS::setCurrScene(const std::string& name) {
        _currScene = getScene(name);
    }
}}
