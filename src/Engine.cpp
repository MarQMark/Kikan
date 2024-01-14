#include <iostream>
#include <sstream>
#include "Kikan/Engine.h"

namespace Kikan {
    Engine* Engine::s_instance = nullptr;

    bool Engine::shouldRun() const {
        return _should_run;
    }

    void Engine::update_fps(){
        //get delta time
        _dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count();
        _last_time = std::chrono::high_resolution_clock::now();

        //if dt is higher than a day -> error -> set to 0
        if(_dt > 1e7)
            _dt = 1e-7;

        _frames_last_second++;

        // set window title
        if(_time_last_second >= 500.0)
        {
            time.fps = _frames_last_second * 2;
            _time_last_second = 0;
            _frames_last_second = 0;
        }

        _time_last_second += _dt;

        time.dt = _dt;
        time.tt += _dt;
    }

    void Engine::update() {
        update_fps();
        update_title();

        if(glfwWindowShouldClose(((StdRenderer*)_renderer)->getWindow())){
            _should_run = false;
            return;
        }

        if(preUpdate) preUpdate(this);

        _input->update();

        // UPDATE
        _ecs->update(_dt);

        if(postUpdate) postUpdate(this);
        if(preRender) preRender(this);

        // update UI
        _ui->update();

        // RENDER
        _renderer->render(_dt);

        if(postRender != nullptr) postRender(this);

        if(_fps != 0){
            auto delay = (int32_t)(2000./(double)_fps - time.dt);
            if(delay < 0)
                kikanPrintW("[WARNING] Overload could not keep target FPS\n");
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    Renderer *Engine::getRenderer() {
        return _renderer;
    }

    Input *Engine::getInput() {
        return _input;
    }

    void Engine::setTitle(std::string &title) {
        _title = title;
    }

    std::string Engine::getTitle() {
        return _title;
    }

    void Engine::update_title() {
        if(_title.empty()){
            std::stringstream ss;
            ss << "KIKAN - FPS: " << time.fps;
            glfwSetWindowTitle(((StdRenderer*)_renderer)->getWindow(), ss.str().c_str());
        }
        else {
            glfwSetWindowTitle(((StdRenderer*)_renderer)->getWindow(), _title.c_str());
        }
    }

    ECS *Engine::getECS() {
        return _ecs;
    }

    UI *Engine::getUI() {
        return _ui;
    }

    void Engine::setTargetFPS(uint32_t fps) {
        _fps = fps;
    }

    uint32_t Engine::getTargetFPS() {
        return _fps;
    }

}

