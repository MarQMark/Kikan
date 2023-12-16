#include "Kikan/renderer/stdRenderer/StdRenderer.h"

#include <iostream>
#include <sstream>

#include "Kikan/util/EarClipping.h"
#include "Kikan/renderer/stdRenderer/Shaders.h"


namespace Kikan { namespace Renderer {

#define signature(x) VertexRegistry::getSignature<x>()

    void window_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void StdRenderer::setup_openGl() {
        /* Initialize the library */
        if (!glfwInit())
            std::cout << "ERROR: Could not initialize GLFW" << std::endl;

        /* Create a windowed mode _window and its OpenGL context */
        _window = glfwCreateWindow(_width, _height, "Kikan", nullptr, nullptr);
        if (!_window) {
            glfwTerminate();
            std::cout << "ERROR: Could create Window" << std::endl;
        }

        /* Make the _window's context current */
        glfwMakeContextCurrent(_window);

        //disable VSYNC
        //glfwSwapInterval(0);

        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        //set window resize event
        glfwSetWindowSizeCallback(_window, window_size_callback);

        if (glewInit() != GLEW_OK)
            std::cout << "ERROR: Could not initialize GLEW" << std::endl;

        //load default shader
        _shaders["default"] = new Shader(StdShaders::sVS, StdShaders::sFS);

        //load texture slot in sampler in default frag shader
        _shaders["default"]->uniform1li("u_sampler", 0);

        //load default vertex layout
        VertexRegistry::addLayout<DefaultVertex>(DefaultVertex::getLayout());

        queryErrors("Setup");
    }

    GLFWwindow *StdRenderer::getWindow() {
        return _window;
    }

    void StdRenderer::render(double dt) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _shaders["default"]->bind();
        _shaders["default"]->uniformM4fv("u_mvp", mvp);

        if (preRender) preRender(_o_pre_render, this, dt);
        if (_override_render) _override_render->preRender(this, dt);

        queryErrors("Update");

        //handle Auto Batches
        for (const auto &batch: _auto_batches) {
            batch.second->render();
            delete batch.second;
        }
        _auto_batches.clear();

        if (postRender) postRender(_o_post_render, this, dt);
        if (_override_render) _override_render->postRender(this, dt);

        /* Swap front and back buffers */
        glfwSwapBuffers(_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    void StdRenderer::renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer) {
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

        auto prio = (uint16_t)(layer * 100.f + 32768);
        autoBatch<DefaultVertex>(vertices, prio);
    }

    void StdRenderer::renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer) {
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

        auto prio = (uint16_t)(layer * 100.f + 32768);
        autoBatch<DefaultVertex>(vertices, prio, &indices);
    }

    void StdRenderer::renderPolygon(std::vector<glm::vec2> &points, glm::vec4 color, float layer) {
        std::vector<IVertex *> vertices(points.size());
        std::vector<DefaultVertex> data(points.size());
        for (int i = 0; i < (int)points.size(); ++i) {
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

        auto prio = (uint16_t)(layer * 100.f + 32768);
        autoBatch<DefaultVertex>(vertices, prio, &indices);
    }

    void StdRenderer::renderTexture2D(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, GLuint textureId, glm::vec4 color, float layer){
        std::vector<IVertex *> vertices(4);

        DefaultVertex v1;
        v1.position = glm::vec3(p1.x, p1.y, layer);
        v1.textureCoords = glm::vec2(0, 1);
        vertices[0] = &v1;

        DefaultVertex v2;
        v2.position = glm::vec3(p2.x, p2.y, layer);
        v2.textureCoords = glm::vec2(1, 1);
        vertices[1] = &v2;

        DefaultVertex v3;
        v3.position = glm::vec3(p3.x, p3.y, layer);
        v3.textureCoords = glm::vec2(1, 0);
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

        auto prio = (uint16_t)(layer * 100.f + 32768);
        autoBatch<DefaultVertex>(vertices, prio, &indices);
    }

    template<class T>
    void StdRenderer::autoBatch(std::vector<IVertex *> vertices, uint16_t prio, std::vector<GLuint> *indices) {
        if(vertices.empty())
            return;

        int start = 0;
        int stop = 0;
        float textureID = vertices[0]->texture;

        for (IVertex* v : vertices) {
            if(v->texture != textureID || v == vertices.back()){
                // get ID
                uint64_t id = auto_batch_id(signature(T), prio, textureID);

                // Create Batch if none with ID exist
                if(!_auto_batches.count(id))
                    _auto_batches[id] = new AutoBatch(VertexRegistry::getLayout<T>(), sizeof(T), textureID);

                // Send vertices to batch
                if(indices == nullptr)
                    _auto_batches[id]->addVertices(vertices, start, stop);
                else
                    _auto_batches[id]->addVertices(vertices, *indices, start, stop);

                // Update start and position
                start = stop + 1;
                textureID = v->texture;
            }

            stop++;
        }
    }

    Shader *StdRenderer::shader(const std::string& name) {
        return _shaders[name];
    }

    void StdRenderer::shader(Shader* shader, const std::string &name) {
        _shaders[name] = shader;
    }

    void StdRenderer::queryErrors(const std::string& tag) {
        GLenum err = glGetError();
        while(err != GL_NO_ERROR){
            std::stringstream ss;
            ss<< std::hex << err; // int decimal_value
            std::string res ( ss.str() );
            std::cout << "[OPENGL ERROR] in " << tag << ": " << res << std::endl;
            err = glGetError();
        }
    }

    void StdRenderer::addPreRender(void (*func)(void *, StdRenderer *, double), void *o) {
        preRender = func;
        _o_pre_render = o;
    }

    void StdRenderer::addPostRender(void (*func)(void *, StdRenderer *, double), void *o) {
        postRender = func;
        _o_post_render = o;
    }

    void StdRenderer::overrideRender(Override* ovr) {
        _override_render = ovr;
    }

    uint64_t StdRenderer::auto_batch_id(uint32_t signature,uint16_t prio, float textureID) {
        return (uint64_t) prio << 48 | signature << 16 | (uint16_t)textureID;
    }


    void StdRenderer::setWidth(int width) {
        glfwSetWindowSize(_window, width, _height);
    }

    int StdRenderer::getWidth() {
        glfwGetWindowSize(_window, &_width, &_height);
        return _width;
    }

    void StdRenderer::setHeight(int height) {
        glfwSetWindowSize(_window, _width, height);
    }

    int StdRenderer::getHeight() {
        glfwGetWindowSize(_window, &_width, &_height);
        return _height;
    }

    void StdRenderer::addBatch(ManuelBatch *batch, unsigned int key) {
        _batches[key] = batch;
    }

    ManuelBatch *StdRenderer::getBatch(unsigned int key) {
        return _batches[key];
    }

    void StdRenderer::destroy() {
        delete this;
    }
} }