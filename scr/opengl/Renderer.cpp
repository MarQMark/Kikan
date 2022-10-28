#include "Renderer.h"
#include <iostream>

#define signature(x) VertexRegistry::getSignature<x>()

void Renderer::setup_openGl() {
    /* Initialize the library */
    if (!glfwInit())
        std::cout << "ERROR: Could not initialize GLFW" << std::endl;

    /* Create a windowed mode _window and its OpenGL context */
    _window = glfwCreateWindow(1280, 720, "Kikan", NULL, NULL);
    if (!_window) {
        glfwTerminate();
        std::cout << "ERROR: Could create Window" << std::endl;
    }

    /* Make the _window's context current */
    glfwMakeContextCurrent(_window);

    //disable VSYNC
    glfwSwapInterval(0);

    if(glewInit() != GLEW_OK)
        std::cout << "ERROR: Could not initialize GLEW" << std::endl;

    _shaders["default"] = new Shader("shaders/default.vert", "shaders/default.frag");
}

GLFWwindow *Renderer::getWindow() {
    return _window;
}

void Renderer::render(double dt) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto batch : _batches)
        batch.second->render();

    //handle Auto Batches
    for (const auto& batches : _auto_batches) {
        for(AutoBatch* batch : batches.second)
        {
            batch->render();
            delete batch;
        }
    }
    _auto_batches.clear();

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);

    /* Poll for and process events */
    glfwPollEvents();
}

void Renderer::renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color) {
    std::vector<IVertex> vertices(3);

    DefaultVertex v1 = DefaultVertex();
    v1.position = glm::vec3(p1.x, p1.y, 0.0f);
    v1.textureCoords = glm::vec2(0.0);
    v1.color = color;
    v1.texture = -1;
    vertices.push_back(v1);

    DefaultVertex v2 = DefaultVertex();
    v2.position = glm::vec3(p2.x, p2.y, 0.0f);
    v2.textureCoords = glm::vec2(0.0);
    v2.color = color;
    v2.texture = -1;
    vertices.push_back(v2);

    DefaultVertex v3 = DefaultVertex();
    v3.position = glm::vec3(p3.x, p3.y, 0.0f);
    v3.textureCoords = glm::vec2(0.0);
    v3.color = color;
    v3.texture = -1;
    vertices.push_back(v3);

    autoBatch<DefaultVertex>(vertices);
}

template <class T>
void Renderer::autoBatch(std::vector<IVertex> vertices) {

    if(_auto_batches.empty()){
        _auto_batches[signature(T)].push_back(new AutoBatch(vertices[0].getLayout(), sizeof(T)));
    }

    int result = _auto_batches[signature(T)].back()->addVertices(vertices);
    while (result != 0) {
        _auto_batches[signature(T)].push_back(new AutoBatch(vertices[0].getLayout(), sizeof(T)));
        std::vector<IVertex> remainingVertices(vertices.begin() + result, vertices.end());
        result = _auto_batches[signature(T)].back()->addVertices(remainingVertices);
    }
}
