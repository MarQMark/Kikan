#include "Kikan/Engine.h"

#include "Kikan/ecs/systems/SpriteRenderSystem.h"
#include "Kikan/ecs/components/Texture2DSprite.h"
#include "Kikan/ecs/components/QuadSprite.h"
#include "Kikan/ecs/components/PolygonSprite.h"
#include "Kikan/ecs/Entity.h"
#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"
#include "Kikan/renderer/stdRenderer/vertices/VertexRegistry.h"
#include "Kikan/renderer/stdRenderer/batching/ManuelBatch.h"

#include "Kikan/core/Logging.h"
#include "Kikan/renderer/stdRenderer/Camera.h"
#include "Kikan/ecs/components/LineQuadSprite.h"
#include "Kikan/ecs/components/Physics.h"
#include "Kikan/ecs/TypeRegistry.h"

#include "Kikan/ecs/Util.h"
#include "Kikan/ecs/components/AASprite.h"

double tt = 0;

void preRender(void* o, Kikan::Renderer::StdRenderer* renderer, double dt){
    tt += dt;
    /*int width, height;
    glfwGetWindowSize(renderer->getWindow(), &width, &height);

    Kikan::Shader shader(StdShaders::sVS, "/home/mark/Documents/shader/ray.frag");
    shader.bind();
    shader.uniformM4fv("u_mvp", renderer->mvp);
    shader.uniform2fv("u_resolution", glm::vec2(width, height));
    shader.uniform1lf("u_time", tt / 1000.0);*/

    {
        std::vector<Kikan::Renderer::DefaultVertex> vertices(4);

        Kikan::Renderer::DefaultVertex v1;
        v1.position = glm::vec3(-.5, .5, -.5);
        vertices[0] = v1;

        Kikan::Renderer::DefaultVertex v2;
        v2.position = glm::vec3(.5, .5, -.5);
        vertices[1] = v2;

        Kikan::Renderer::DefaultVertex v3;
        v3.position = glm::vec3(.5, -.5, -.5);
        vertices[2] = v3;

        Kikan::Renderer::DefaultVertex v4;
        v4.position = glm::vec3(-.5, -.5, -.5);
        vertices[3] = v4;

        for (auto& v : vertices) {
            v.textureCoords = glm::vec2(0.0);
            v.color = glm::vec4(0.2f, std::sin(tt / 1000.f), .4f, 1.f);
            v.texture = -1;
        }

        std::vector<GLuint> indices = {0, 1, 2, 0, 2, 3};

        renderer->getBatch(0)->overrideVertices<Kikan::Renderer::DefaultVertex>(vertices, indices);
    }

    renderer->getBatch(0)->render();
}

int WinMain() {
    Kikan::Engine::init();
    Kikan::Engine* engine = Kikan::Engine::Kikan();

    Kikan::Renderer::Camera camera;
    ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->mvp = camera.matrix();

    engine->getECS()->getScene()->addSystem(new Kikan::SpriteRenderSystem());

   // ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->addPostRender(preRender, nullptr);

    {
        auto* entity = new Kikan::Entity();
        auto* sprite = new Kikan::LineQuadSprite();
        entity->getComponent<Kikan::Transform>()->position = glm::vec3(-.5, .5, 0);
        sprite->dimensions = glm::vec2(1, 1);
        sprite->color = glm::vec4(0.8, 0.4, 0.3, 1.0);
        sprite->layer = 0.1;
        sprite->thickness = 0.01;
        entity->addComponent(sprite);
        engine->getECS()->getScene()->addEntity(entity);
    }

    {
        auto* entity = new Kikan::Entity();
        auto* physics = new Kikan::Physics();
        entity->addComponent(physics);
        engine->getECS()->getScene()->addEntity(entity);
    }

    {
        auto* entity = new Kikan::Entity();
        auto* sprite = new Kikan::LineQuadSprite();
        auto* physics = new Kikan::Physics();
        entity->addComponent(sprite);
        entity->addComponent(physics);
        engine->getECS()->getScene()->addEntity(entity);
    }

    {
        auto* entity = new Kikan::Entity();
        auto* sprite = new Kikan::LineQuadSprite();
        entity->addComponent(sprite);
        engine->getECS()->getScene()->addEntity(entity);
    }

    {
        auto* entity = new Kikan::Entity();
        auto* sprite = new Kikan::AASprite();
        sprite->offset = glm::vec2(-.5, .5);
        sprite->dimensions = glm::vec2(1);
        sprite->textureID = ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->getFont()->getID();
        sprite->layer = -.2f;
        entity->addComponent(sprite);
        engine->getECS()->getScene()->addEntity(entity);
    }


    std::vector<Kikan::Entity*> es;
    engine->getECS()->getScene()->getEntities(getSig(Kikan::Transform), &es);
    //Kikan::Entity* e = engine->getECS()->getScene()->getEntity(getSigs(sig(Kikan::Physics), sig(Kikan::LineQuadSprite)));

    while (engine->shouldRun()) {
        engine->update();
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif
