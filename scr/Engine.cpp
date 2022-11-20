#include <iostream>
#include <sstream>
#include "Engine.h"

bool Engine::shouldRun() const {
    return _should_run;
}

Scene *Engine::getScene(const std::string& name) {

    //if no scene exists create default scene
    if(_scenes.capacity() == 0){
        Scene* defaultScene = new Scene("default", _renderer);
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


void Engine::addScene(const std::string& name) {
    //TODO implement function
}

void Engine::updateFPS(){
    //get delta time
    _dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count();
    _last_time = std::chrono::high_resolution_clock::now();

    //if dt is higher than a day -> error -> set to 0
    if(_dt > 1e5)
        _dt = 0;

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

    if(preUpdate) preUpdate(this);

    // UPDATE
    for (Scene* scene : _scenes)
        scene->update(_dt);

    if(postUpdate) postUpdate(this);
    if(preRender) preRender(this);

    // RENDER
    _renderer->render(_dt);

    if(postRender != nullptr) postRender(this);
}

Renderer *Engine::getRenderer() {
    return _renderer;
}

