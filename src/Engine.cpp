#include <iostream>
#include <sstream>
#include "Kikan/Engine.h"
#include "Kikan/core/Logging.h"

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

    void Engine::limit_fps() {
        if(_fps == 0)
            return;

        auto delay = (double)(1000./(double)_fps - ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - _last_time)).count());
        if(delay < 0)
            kikanPrintW("[WARNING] Overload could not keep target FPS\n");
        else
            std::this_thread::sleep_for(std::chrono::microseconds((uint32_t)(delay * 1000)));
    }

    void Engine::update() {
        update_fps();
        update_title();

#if KIKAN_RENDERER
        if(((IStdRenderer*)_renderer)->shouldClose()){
            _should_run = false;
            return;
        }
#endif

        if(preUpdate) preUpdate(this);

#if KIKAN_INPUT
        _input->update();
#endif

#if KIKAN_ECS
        // UPDATE
        _ecs->update(_dt);
#endif

        if(postUpdate) postUpdate(this);
        if(preRender) preRender(this);

#if KIKAN_UI
        // update UI
        _ui->update();
#endif

#if KIKAN_RENDERER
        // RENDER
        _renderer->render(_dt);
#endif

        if(postRender != nullptr) postRender(this);

        limit_fps();
    }

#if KIKAN_RENDERER
    Renderer *Engine::getRenderer() {
        return _renderer;
    }
#endif

#if KIKAN_INPUT
    IStdInput *Engine::getInput() {
        return _input;
    }
#endif

    void Engine::setTitle(std::string &title) {
        _title = title;
    }

    std::string Engine::getTitle() {
        return _title;
    }

    void Engine::update_title() {
#if KIKAN_RENDERER
        if(_title.empty()){
            std::stringstream ss;
            ss << "KIKAN - FPS: " << time.fps;
            ((IStdRenderer*)_renderer)->setTitle(ss.str().c_str());
        }
        else {
            ((IStdRenderer*)_renderer)->setTitle(_title.c_str());
        }
#endif
    }

#if KIKAN_ECS
    ECS *Engine::getECS() {
        return _ecs;
    }
#endif

#if KIKAN_UI
    UI *Engine::getUI() {
        return _ui;
    }
#endif

    void Engine::setTargetFPS(uint32_t fps) {
        _fps = fps;
    }

    uint32_t Engine::getTargetFPS() const {
        return _fps;
    }

}

