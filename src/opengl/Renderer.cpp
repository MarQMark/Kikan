#include "Kikan/opengl/Renderer.h"
#include <iostream>
#include "Kikan/util/EarClipping.h"

namespace Kikan {

#define signature(x) VertexRegistry::getSignature<x>()

    void window_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

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

        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        //set window resize event
        glfwSetWindowSizeCallback(_window, window_size_callback);

        if (glewInit() != GLEW_OK)
            std::cout << "ERROR: Could not initialize GLEW" << std::endl;

        //load default shader
        _shaders["default"] = new Shader("shaders/default.vert", "shaders/default.frag");

        //load max texture units in sampler in default frag shader
        GLint maxTextureUnits;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
        GLint textures[maxTextureUnits];
        for(int i = 0; i < maxTextureUnits; i++) textures[i] = i;
        _shaders["default"]->uniform1iv("u_texture", 8, textures);

        //load default vertex layout
        VertexRegistry::addLayout<DefaultVertex>(DefaultVertex::getLayout());
    }

    GLFWwindow *Renderer::getWindow() {
        return _window;
    }

    void Renderer::render(double dt) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _shaders["default"]->bind();
        _shaders["default"]->uniformM4fv("u_mvp", mvp);

        if (preRender) preRender(this, dt);

        for (auto batch: _batches)
            batch.second->render();

        while(GLenum err = glGetError() != GL_NO_ERROR){
            std::cout << "ERROR: " << err << std::endl;
        }

        //handle Auto Batches
        for (const auto &batches: _auto_batches) {
            for (AutoBatch *batch: batches.second) {
                batch->render();
                delete batch;
            }
        }
        _auto_batches.clear();

        if (postRender) postRender(this, dt);

        /* Swap front and back buffers */
        glfwSwapBuffers(_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

/*
 *  Uses Auto-batching with DefaultVertex.
 */
    void Renderer::renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer) {
        std::vector<IVertex *> vertices(3);

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
 */
    void Renderer::renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer) {
        std::vector<IVertex *> vertices(4);

        DefaultVertex v1;
        v1.position = glm::vec3(p1.x, p1.y, layer);
        vertices[0] = &v1;

        DefaultVertex v2;
        v2.position = glm::vec3(p2.x, p2.y, layer);
        vertices[1] = &v2;

        DefaultVertex v3;
        v3.position = glm::vec3(p3.x, p3.y, layer);
        vertices[2] = &v3;

        DefaultVertex v4;
        v4.position = glm::vec3(p4.x, p4.y, layer);
        vertices[3] = &v4;

        for (auto *v: vertices) {
            ((DefaultVertex *) v)->textureCoords = glm::vec2(0.0);
            ((DefaultVertex *) v)->color = color;
            v->texture = -1;
        }

        std::vector<GLuint> indices = {0, 1, 2, 0, 2, 3};

        autoBatch<DefaultVertex>(vertices, indices);
    }

/*
 *  Uses Auto-batching with DefaultVertex.
 *
 *  Uses Ear-Clipping-Algorithm to divide into Triangles.
 *  This means Polygons cannot intersect with themselves, cannot have holes and
 *  three or more vertices cannot form a line
 */
    void Renderer::renderPolygon(std::vector<glm::vec2> &points, glm::vec4 color, float layer) {
        std::vector<IVertex *> vertices(points.size());
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
        if (result < 0)
            std::cout << "[ERROR] Could not triangulate Polygon" << std::endl;

        autoBatch<DefaultVertex>(vertices, indices);
    }

    /*
     * Uses Auto-batching with DefaultVertex.
     *
     * Order of Points is:
     *      1 ------ 2
     *      |        |
     *      |        |
     *      4 ------ 3
     */
    void Renderer::renderTexture2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, GLuint textureId, glm::vec4 color, float layer){
        std::vector<IVertex *> vertices(4);

        DefaultVertex v1;
        v1.position = glm::vec3(p1.x, p1.y, layer);
        v1.textureCoords = glm::vec2(1, 0);
        vertices[0] = &v1;

        DefaultVertex v2;
        v2.position = glm::vec3(p2.x, p2.y, layer);
        v2.textureCoords = glm::vec2(1, 1);
        vertices[1] = &v2;

        DefaultVertex v3;
        v3.position = glm::vec3(p3.x, p3.y, layer);
        v3.textureCoords = glm::vec2(0, 1);
        vertices[2] = &v3;

        DefaultVertex v4;
        v4.position = glm::vec3(p4.x, p4.y, layer);
        v4.textureCoords = glm::vec2(0, 0);
        vertices[3] = &v4;

        for (auto *v: vertices) {
            ((DefaultVertex *) v)->color = color;
            v->texture = (float)textureId;
        }

        std::vector<GLuint> indices = {0, 1, 2, 0, 2, 3};

        autoBatch<DefaultVertex>(vertices, indices);
    }

    template<class T>
    void Renderer::autoBatch(std::vector<IVertex *> vertices) {
        if (_auto_batches.empty())
            _auto_batches[signature(T)].push_back(new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T)));

        int result = _auto_batches[signature(T)].back()->addVertices(vertices);
        while (result != 0) {
            _auto_batches[signature(T)].push_back(new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T)));
            std::vector<IVertex *> remainingVertices(vertices.begin() + result, vertices.end());
            result = _auto_batches[signature(T)].back()->addVertices(remainingVertices);
        }
    }

    template<class T>
    void Renderer::autoBatch(std::vector<IVertex *> vertices, std::vector<GLuint> &indices) {
        if (_auto_batches.empty())
            _auto_batches[signature(T)].push_back(new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T)));

        int result = _auto_batches[signature(T)].back()->addVertices(vertices, indices);
        if (result == -1)
            std::cout << "[ERROR] cannot auto-batch specified vertices with custom indices" << std::endl;
    }

    Shader *Renderer::shader(const std::string& name) {
        return _shaders[name];
    }
}