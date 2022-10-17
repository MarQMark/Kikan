#ifndef KIKAN_ENGINE_H
#define KIKAN_ENGINE_H

#include "vector"
#include "ecs/Scene.h"
#include "opengl/Renderer.h"

class Engine {
public:
    Engine(){
        _renderer = new Renderer();
    }

    ~Engine(){
        delete _renderer;
    }

    void update(double dt);
    Scene* getScene(const std::string&);
    Renderer* getRenderer();

private:
    Renderer* _renderer;

    std::vector<Scene*> _scenes;
};


#endif //KIKAN_ENGINE_H
