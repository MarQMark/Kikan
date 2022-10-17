#include "Engine.h"

Scene *Engine::getScene(const std::string& name) {

    //if no scene exists create default scene
    if(_scenes.capacity() == 0){
        Scene* defaultScene = new Scene("default");
        _scenes.push_back(defaultScene);
        return defaultScene;
    }

    //look for specified scene
    for (Scene* scene : _scenes) {
        if (scene->getName() == name)
            return scene;
    }

    //return default if no scene found
    return _scenes.at(0);
}

void Engine::update(double dt) {
    for (Scene* scene : _scenes) {
        scene->update(dt);
    }
}

Renderer *Engine::getRenderer() {
    return _renderer;
}
