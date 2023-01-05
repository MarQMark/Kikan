#ifndef KIKAN_SCENE_H
#define KIKAN_SCENE_H

#include <utility>
#include <vector>
#include "string"
#include "Entity.h"
#include "systems/ISystem.h"
#include "../opengl/Renderer.h"
#include "../opengl/Camera.h"

namespace Kikan {
    class Scene {
    public:
        Scene(std::string name, Renderer* renderer)
            : _name(std::move(name)), _renderer(renderer){

            int width, height;
            glfwGetWindowSize(renderer->getWindow(), &width, &height);
            _camera = new Camera();
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

        Camera* camera();

        void update(double dt);

        void addEntity(Entity* entity);
        void removeEntity(Entity* entity);
        void deleteEntity(Entity* entity);
        void addSystem(ISystem* entity);

    private:
        const std::string _name;
        Renderer* _renderer;
        Camera* _camera;

        std::vector<Entity*> _entities;
        std::vector<ISystem*> _systems;
    };
}

#endif //KIKAN_SCENE_H
