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

    bool shouldRun() const;

    void update(double dt);
    Scene* getScene(const std::string& = "default");
    Renderer* getRenderer();

private:
    bool _should_run = true;

    Renderer* _renderer;
    std::vector<Scene*> _scenes;
};


#endif //KIKAN_ENGINE_H
