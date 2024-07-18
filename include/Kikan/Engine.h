#ifndef KIKAN_ENGINE_H
#define KIKAN_ENGINE_H

#include "Features.h"

#include <chrono>
#include <string>
#include <vector>

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
    public:
        struct InitParams {
            InitParams(){
                pRenderer = nullptr;
                pRendererParams = nullptr;
                pECS = nullptr;
                pECSParams = nullptr;
                pInput = nullptr;
                pInputParams = nullptr;
                pUI = nullptr;
                pUIParams = nullptr;
            }

            void* pRenderer;
            void* pRendererParams;
            void* pECS;
            void* pECSParams;
            void* pInput;
            void* pInputParams;
            void* pUI;
            void* pUIParams;
        };

    private:
        Engine(struct InitParams& params){

#if KIKAN_RENDERER
            if(params.pRenderer)
                _renderer = (Renderer*)params.pRenderer;
            else{
                _renderer = new StdRenderer(params.pRendererParams);

#if KIKAN_INPUT
                if(!params.pInput){
                    auto inputParams = new Input::InitParams;
                    inputParams->window = ((StdRenderer*)_renderer)->getWindow();
                    params.pInputParams = inputParams;
                }
#endif

            }
#endif
#if KIKAN_INPUT
            if(params.pInput)
                _input = (Input*)params.pInput;
            else
                _input = Input::create(params.pInputParams);
#endif
#if KIKAN_ECS
            if(params.pECS)
                _ecs = (ECS*)params.pECS;
            else
                _ecs = new ECS(params.pECSParams);
#endif
#if KIKAN_UI
            if(params.pUI)
                _ui = (UI*)params.pUI;
            else
                _ui = new UI(params.pUIParams);
#endif
        }

        static Engine* s_instance;
    public:



        static void init(struct InitParams params = InitParams()){
            s_instance = new Engine(params);
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
