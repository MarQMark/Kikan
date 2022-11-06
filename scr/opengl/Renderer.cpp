#include "Renderer.h"
#include <iostream>
#include "../util/EarClipping.h"

#define signature(x) VertexRegistry::getSignature<x>()

void Renderer::setup_openGl() {
    /* Initialize the library */
    if (!glfwInit())
        std::cout << "ERROR: Could not initialize GLFW" << std::endl;

    /* Create a windowed mode _window and its OpenGL context */
    _window = glfwCreateWindow(1280, 720, "Kikan", nullptr, nullptr);
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
    VertexRegistry::addLayout<DefaultVertex>(DefaultVertex::getLayout());
}

GLFWwindow *Renderer::getWindow() {
    return _window;
}

void Renderer::render(double dt) {
    glClear(GL_COLOR_BUFFER_BIT);

    _shaders["default"]->bind();

    for (auto batch : _batches)
        batch.second->render();

    while(GLenum err = glGetError() != GL_NO_ERROR){
        std::cout << "ERROR: " << err << std::endl;
    }

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

/*
 *  Uses Auto-batching with DefaultVertex.
 */
void Renderer::renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, int layer) {
    std::vector<IVertex*> vertices(3);

    DefaultVertex v1;
    v1.position = glm::vec3(p1.x, p1.y, layer);
    v1.textureCoords = glm::vec2(0.0);
    v1.color = color;
    v1.texture = -1;
    vertices[0] = &v1;

    DefaultVertex v2;
    v2.position = glm::vec3(p2.x, p2.y, layer);
    v2.textureCoords = glm::vec2(0.0);
    v2.color = color;
    v2.texture = -1;
    vertices[1] = &v2;

    DefaultVertex v3;
    v3.position = glm::vec3(p3.x, p3.y, layer);
    v3.textureCoords = glm::vec2(0.0);
    v3.color = color;
    v3.texture = -1;
    vertices[2] = &v3;

    autoBatch<DefaultVertex>(vertices);
}

/*
 *  Uses Auto-batching with DefaultVertex.
 *
 *  Uses Ear-Clipping-Algorithm to divide into Triangles.
 *  This means Polygons cannot intersect with themselves, cannot have holes and
 *  three or more vertices cannot form a line
 */
void Renderer::renderPolygon(std::vector<glm::vec2>& points, glm::vec4 color, int layer) {
    std::vector<IVertex*> vertices(points.size());
    std::vector<DefaultVertex> data(points.size());
    for (int i = 0; i < points.size(); ++i) {
        data[i].position = glm::vec3(points[i].x, points[i].y, 0.0f);
        data[i].textureCoords = glm::vec2(0.0);
        data[i].color = color;
        data[i].texture = -1;
        vertices[i] = &data[i];
    }

    std::vector<GLuint> indices((points.size() - 2) * 3);
    int result = triangulate(points, indices);
    if(result < 0)
        std::cout << "[ERROR] Could not triangulate Polygon" << std::endl;

    autoBatch<DefaultVertex>(vertices, indices);
}

template <class T>
void Renderer::autoBatch(std::vector<IVertex*> vertices) {
    if(_auto_batches.empty())
        _auto_batches[signature(T)].push_back(new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T)));

    int result = _auto_batches[signature(T)].back()->addVertices(vertices);
    while (result != 0) {
        _auto_batches[signature(T)].push_back(new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T)));
        std::vector<IVertex*> remainingVertices(vertices.begin() + result, vertices.end());
        result = _auto_batches[signature(T)].back()->addVertices(remainingVertices);
    }
}

template<class T>
void Renderer::autoBatch(std::vector<IVertex *> vertices, std::vector<GLuint> &indices) {
    if(_auto_batches.empty())
        _auto_batches[signature(T)].push_back(new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T)));

    int result = _auto_batches[signature(T)].back()->addVertices(vertices, indices);
    if(result == -1)
        std::cout << "[ERROR] cannot auto-batch specified vertices with custom indices" << std::endl;
}
