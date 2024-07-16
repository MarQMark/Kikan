#ifndef KIKAN_ENGINE_H
#define KIKAN_ENGINE_H

#include "Features.h"

#include <chrono>
#include "vector"

#include "Kikan/util/Time.h"

#if KIKAN_RENDERER
#include "Kikan/renderer/stdRenderer/StdRenderer.h"
#endif
#if KIKAN_ECS
#include "Kikan/ecs/ECS.h"
#endif
#if KIKAN_INPUT
#include "Kikan/input/Input.h"
#endif
#if KIKAN_UI
#include "Kikan/ui/UI.h"
#endif

namespace Kikan {
    class Engine {
    private:
        Engine(int width, int height){

#if KIKAN_RENDERER
            _renderer = new StdRenderer(width, height);
#endif
#if KIKAN_INPUT
            _input = Input::create(((StdRenderer*)_renderer)->getWindow());
#endif
#if KIKAN_ECS
            _ecs = new ECS();
#endif
#if KIKAN_UI
            _ui = new UI();
#endif
        }

        static Engine* s_instance;
    public:
        static void init(int width = 1280, int height = 720){
            s_instance = new Engine(width, height);
        }
        static Engine* Kikan() {
            return s_instance;
        }

        ~Engine(){
#if KIKAN_INPUT
            delete _input;
#endif
#if KIKAN_RENDERER
            _renderer->destroy();
#endif
        }

        bool shouldRun() const;

        void update();

#if KIKAN_RENDERER
        Renderer* getRenderer();
#endif
#if KIKAN_ECS
        ECS* getECS();
#endif
#if KIKAN_INPUT
        Input* getInput();
#endif
#if KIKAN_UI
        UI* getUI();
#endif

        Time time;

        void setTitle(std::string& title);
        std::string getTitle();

        void setTargetFPS(uint32_t fps);
        uint32_t getTargetFPS() const;

        // custom methods
        void (*preUpdate)(Engine*) = nullptr;
        void (*postUpdate)(Engine*) = nullptr;
        void (*preRender)(Engine*) = nullptr;
        void (*postRender)(Engine*) = nullptr;
    private:
        bool _should_run = true;

        std::chrono::high_resolution_clock::time_point _last_time;
        double _dt = 0;
        double _time_last_second = 0;
        unsigned int _frames_last_second = 0;

        std::string _title = std::string();
        uint32_t _fps = 60;

#if KIKAN_RENDERER
        Renderer* _renderer;
#endif
#if KIKAN_ECS
        ECS* _ecs;
#endif
#if KIKAN_INPUT
        Input* _input;
#endif
#if KIKAN_UI
        UI* _ui;
#endif

        void update_fps();
        void limit_fps();
        void update_title();
    };
}

#endif //KIKAN_ENGINE_H
