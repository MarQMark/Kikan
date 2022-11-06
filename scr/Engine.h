#ifndef KIKAN_ENGINE_H
#define KIKAN_ENGINE_H

#include <chrono>
#include "vector"
#include "ecs/Scene.h"
#include "opengl/Renderer.h"

class Engine {
public:
    static Engine* get();

    Engine(){
        _renderer = new Renderer();
    }

    ~Engine(){
        delete _renderer;
    }

    bool shouldRun() const;

    void update();
    Scene* getScene(const std::string& = "default");
    Renderer* getRenderer();

    // custom methods
    void (*preUpdate)(double) = nullptr;
    void (*postUpdate)(double) = nullptr;
    void (*preRender)(double) = nullptr;
    void (*postRender)(double) = nullptr;
private:
    static Engine* _engine;

    bool _should_run = true;

    std::chrono::high_resolution_clock::time_point _last_time;
    double _dt = 0;
    double _time_last_second = 0;
    unsigned int _frames_last_second = 0;

    Renderer* _renderer;
    std::vector<Scene*> _scenes;

    void updateFPS();
};


#endif //KIKAN_ENGINE_H
