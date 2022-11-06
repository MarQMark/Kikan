#include <iostream>
#include <sstream>
#include "Engine.h"

Engine* Engine::_engine = nullptr;

Engine *Engine::get() {
    if(_engine == nullptr)
        _engine = new Engine();

    return _engine;
}

bool Engine::shouldRun() const {
    return _should_run;
}

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

void Engine::updateFPS(){
    //get delta time
    _dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count();
    _last_time = std::chrono::high_resolution_clock::now();

    _frames_last_second++;

    // set window title
    if(_time_last_second >= 500.0)
    {
        std::stringstream ss;
        ss << "KIKAN - FPS: " << _frames_last_second * 2;
        glfwSetWindowTitle(_renderer->getWindow(), ss.str().c_str());

        _time_last_second = 0;
        _frames_last_second = 0;
    }

    _time_last_second += _dt;
}

void Engine::update() {
    updateFPS();

    if(glfwWindowShouldClose(_renderer->getWindow())){
        _should_run = false;
        return;
    }

    if(preUpdate) preUpdate(_dt);

    // UPDATE
    for (Scene* scene : _scenes)
        scene->update(_dt);

    if(postUpdate) postUpdate(_dt);
    if(preRender) preRender(_dt);

    // RENDER
    _renderer->render(_dt);

    if(postRender != nullptr) postRender(_dt);
}

Renderer *Engine::getRenderer() {
    return _renderer;
}