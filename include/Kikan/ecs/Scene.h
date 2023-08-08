#ifndef KIKAN_SCENE_H
#define KIKAN_SCENE_H

#include <utility>
#include <vector>
#include "string"
#include "Kikan/ecs/Entity.h"
#include "Kikan/renderer/stdRenderer/StdRenderer.h"
#include "Kikan/ecs/systems/ISystem.h"
#include "Kikan/renderer/stdRenderer/Camera.h"

namespace Kikan {
    class Scene {
    public:
        Scene(std::string name, Renderer::StdRenderer* renderer, Input* input)
            : _name(std::move(name)), _renderer(renderer), _input(input){

            int width, height;
            glfwGetWindowSize(renderer->getWindow(), &width, &height);
            _camera = new Renderer::Camera();
        }

        ~Scene(){
            delete _camera;

            for(Entity* entity : _entities)
                delete entity;

            // TODO: Fix probably memory leak
            for(ISystem* system : _systems)
                delete system;
        }

        std::string name();

        Renderer::Camera* camera();

        void update(double dt);

        void addEntity(Entity* entity);
        void removeEntity(Entity* entity);
        void deleteEntity(Entity* entity);
        void addSystem(ISystem* entity);

    private:
        const std::string _name;
        Renderer::StdRenderer* _renderer;
        Renderer::Camera* _camera;
        Input* _input;

        std::vector<Entity*> _entities;
        std::vector<ISystem*> _systems;
    };
}

#endif //KIKAN_SCENE_H
