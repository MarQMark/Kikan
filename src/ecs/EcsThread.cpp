#include "Kikan/ecs/EcsThread.h"
#include "Kikan/core/Logging.h"
#include "Kikan/ecs/Util.h"
#include <chrono>

namespace Kikan { namespace Ecs {
    Kikan::Ecs::EcsThread::~EcsThread() {
        for (auto *system: _systems) {
            delete system;
        }
        if (_status != EcsThread::STOPPED) {
            pthread_cancel(_t->native_handle());
        }
        delete _t;
    }

    Kikan::Ecs::EcsThread::EcsThread(uint32_t cycleTime, uint32_t timout)
            : _cycleTime(cycleTime), _timout(timout) {
        _maxTimeout = timout;
    }

    void EcsThread::run() {
        _status = RUNNING;

        while(_shouldRun){

            auto timeStart = std::chrono::high_resolution_clock::now();
            update();
            double dt = ((std::chrono::duration<double, std::milli>)(std::chrono::high_resolution_clock::now() - timeStart)).count();

            int32_t delay = _cycleTime - dt;
            if(delay < 0)
                kikanPrintW("Thread %i could not keep cycle time\n", std::this_thread::get_id());
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        _status = STOPPED;
    };

    void EcsThread::update() {
        for(auto* system : _systems)
            system->update(_cycleTime);
    }

    void EcsThread::start() {
        _t = new std::thread(&EcsThread::run, this);
    }

    void EcsThread::stop() {
        _shouldRun = false;
    }

    EcsThread::Status EcsThread::getStatus() {
        return _status;
    }

    void EcsThread::addSystem(ISystem *system) {
        _systems.push_back(system);
    }

    void EcsThread::addEntity(Entity *entity) {
        Util::addEntity(entity, &_entities, &_systems);
    }

    void EcsThread::removeEntity(Entity *entity) {
        Util::removeEntity(entity, &_entities, &_systems);
    }

    }}