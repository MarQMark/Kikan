#ifndef KIKAN_ENGINE_H
#define KIKAN_ENGINE_H

#include <chrono>
#include "vector"
#include "Kikan/renderer/stdRenderer/StdRenderer.h"
#include "Kikan/util/Time.h"
#include "Kikan/ecs/ECS.h"
#include "Kikan/input/Input.h"
#include "Kikan/ui/UI.h"

namespace Kikan {
    class Engine {
    private:
        Engine(int width, int height){
            _renderer = new StdRenderer(width, height);
            _input = Input::create(((StdRenderer*)_renderer)->getWindow());
            _ecs = new ECS();
            _ui = new UI();
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
            delete _input;
            _renderer->destroy();
        }

        bool shouldRun() const;

        void update();


        Renderer* getRenderer();
        ECS* getECS();
        Input* getInput();
        UI* getUI();

        Time time;

        void setTitle(std::string& title);
        std::string getTitle();

        void setTargetFPS(uint32_t fps);
        uint32_t getTargetFPS();

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

        Renderer* _renderer;
        ECS* _ecs;
        Input* _input;
        UI* _ui;

        void update_fps();
        void update_title();
    };
}

#endif //KIKAN_ENGINE_H
