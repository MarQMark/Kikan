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

    ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->addPostRender(preRender, nullptr);

    int a = 3;
    Kikan::Core::Logging::get()->print("test %d \n", a);

    auto* batch = new Kikan::Renderer::ManuelBatch(Kikan::Renderer::VertexRegistry::getLayout<Kikan::Renderer::DefaultVertex>(),sizeof(Kikan::Renderer::DefaultVertex));
    ((Kikan::Renderer::StdRenderer*)engine->getRenderer())->addBatch(batch, 0);

    std::vector<float> data(500 * 500 * 4);
    for (int x = 0; x < 500; ++x) {
        for (int y = 0; y < 500; ++y) {
            data[(x + 500 * y) * 4] = 0.;
            data[(x + 500 * y) * 4 + 1] = 0.;
            data[(x + 500 * y) * 4 + 2] = (float)(1.f - std::sqrt((x - 250) * (x - 250) + (y - 250) * (y - 250)) / 250.0);
            data[(x + 500 * y) * 4 + 3] = 1.;
        }
    }

    Kikan::Renderer::Texture2D texture2D(500, 500, data.data());

    auto* entity = new Kikan::Entity();
    auto* sprite = new Kikan::Texture2DSprite;
    sprite->points[0] = glm::vec2(-1, 1);
    sprite->points[1] = glm::vec2(1, 1);
    sprite->points[2] = glm::vec2(1, -1);
    sprite->points[3] = glm::vec2(-1, -1);
    sprite->textureID = texture2D.get();
    sprite->color = glm::vec4(0.3, 0.4, 0.8, 1.0);
    sprite->layer = 0;
    entity->addComponent(sprite);
    engine->getECS()->getScene()->addEntity(entity);

    /*auto* entity2 = new Entity();
    auto* sprite2 = new PolygonSprite();
    std::vector<glm::vec2> points(6);
    points[0] = glm::vec2(-.5, .5);
    points[1] = glm::vec2(0, 1);
    points[2] = glm::vec2(.5, .5);
    points[3] = glm::vec2(.5, -.5);
    points[4] = glm::vec2(0, -1);
    points[5] = glm::vec2(-.5, -.5);
    sprite2->points = points;
    sprite2->color = glm::vec4(0.8, 0.4, 0.3, 1.0);
    sprite2->layer = 0.1;
    entity2->addComponent(sprite2);
    engine.getScene()->addEntity(entity2);*/

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
