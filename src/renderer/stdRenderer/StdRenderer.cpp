#include "Kikan/renderer/stdRenderer/StdRenderer.h"

#include <iostream>
#include <sstream>

#include "Kikan/util/EarClipping.h"
#include "Kikan/renderer/stdRenderer/Shaders.h"
#include "Kikan/renderer/stdRenderer/DefaultFont.h"
#include "Kikan/renderer/stdRenderer/RenderCallbacks.h"
#include "Kikan/core/Logging.h"


namespace Kikan {

#define signature(x) VertexRegistry::getSignature<x>()

    void window_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void StdRenderer::setup_openGl() {

        if(!_deferred){
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
            glfwSwapInterval(0);

            glEnable(GL_DEPTH_TEST);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);

            //set window resize event
            glfwSetWindowSizeCallback(_window, window_size_callback);

            if (glewInit() != GLEW_OK)
                std::cout << "ERROR: Could not initialize GLEW" << std::endl;
        }
        else{
            glGenFramebuffers(1, &_deferred_fbo);
            _deferred_txt = new Texture2D(_width, _height, nullptr);
        }


        //load default shader
        _shaders["default"] = new Shader(StdShaders::sVS, StdShaders::sFS);

        // load default font shader
        _shaders["default_text"] = new Shader(StdShaders::sVS, StdShaders::sTextFS);

        //load texture slot in sampler in default frag shader
        _shaders["default"]->uniform1li("u_sampler", 0);

        //load default vertex layout
        VertexRegistry::addLayout<DefaultVertex>(DefaultVertex::getLayout());

        //load default font
        _fonts["default"] = new Font((void*)defaultFont);

        queryErrors("Setup");
    }

    GLFWwindow *StdRenderer::getWindow() {
        return _window;
    }

    void StdRenderer::render(double dt) {
        if(_deferred){
            glBindFramebuffer(GL_FRAMEBUFFER, _deferred_fbo);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _deferred_txt->get(), 0);
            glViewport(0, 0, _width, _height);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        if(_deferred) {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else{
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }

    void StdRenderer::renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer, Options* opt) {
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

        autoBatch<DefaultVertex>(vertices, getRenderPrio(layer), nullptr, opt);
    }

    void StdRenderer::renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer, Options* opt) {
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
        autoBatch<DefaultVertex>(vertices, getRenderPrio(layer), &indices, opt);
    }

    void StdRenderer::renderPolygon(std::vector<glm::vec2> &points, glm::vec4 color, float layer, Options* opt) {
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

        autoBatch<DefaultVertex>(vertices, getRenderPrio(layer), &indices, opt);
    }

    void StdRenderer::renderTexture2D(glm::vec2 p[4], glm::vec2 texCoords[4], GLuint textureId, glm::vec4 color, float layer, Options* opt){
        std::vector<IVertex *> vertices(4);

        DefaultVertex v1;
        v1.position = glm::vec3(p[0].x, p[0].y, layer);
        v1.textureCoords = texCoords[0];
        vertices[0] = &v1;

        DefaultVertex v2;
        v2.position = glm::vec3(p[1].x, p[1].y, layer);
        v2.textureCoords = texCoords[1];
        vertices[1] = &v2;

        DefaultVertex v3;
        v3.position = glm::vec3(p[2].x, p[2].y, layer);
        v3.textureCoords = texCoords[2];
        vertices[2] = &v3;

        DefaultVertex v4;
        v4.position = glm::vec3(p[3].x, p[3].y, layer);
        v4.textureCoords = texCoords[3];
        vertices[3] = &v4;

        for (auto *v: vertices) {
            ((DefaultVertex *) v)->color = color;
            v->texture = (float)textureId;
        }

        std::vector<GLuint> indices = {0, 1, 2, 0, 2, 3};
        autoBatch<DefaultVertex>(vertices, getRenderPrio(layer), &indices, opt);
    }

    void StdRenderer::renderText(std::string text, glm::vec2 pos, float height, float layer, Font::Options options, Options* opt){
        Font* font = options.font;
        if(!font)
            font = _fonts["default"];

        if(!opt){
            opt = new Options;
            opt->autoFree = true;
            opt->preRender = defaultTextPreRender;
        }

        Font::Glyph* g = font->getGlyph('A');
        const float scale = height/g->dim.y;
        const float whitespace = scale * g->dim.x;

        uint32_t textLen = 0;
        for (char c : text) {
            if(c != ' ' && c != '\t' && c != '\r' && c != '\n')
                textLen++;
        }

        std::vector<DefaultVertex> vertices(4 * textLen);
        std::vector<GLuint> indices(6 * textLen);

        GLuint indexCnt = 0;
        uint32_t nVertex = 0;
        float y = pos.y;
        float x = pos.x;

        for (char c : text) {
            if(c == ' ')       { x += whitespace;      continue; } // Whitespace
            else if(c == '\t') { x += whitespace * 4;  continue; } // Tab
            else if(c == '\r') { x = pos.x;            continue; } // Carriage Return
            else if(c == '\n') {                                   // Newline
                x = pos.x;
                y-= height * 1.5f * options.spacing.y;
                continue;
            }

            g = font->getGlyph(c);
            float cWidth = g->dim.x * scale;
            float cHeight = g->dim.y * scale;
            float offX = g->offset.x * scale;
            float offY = g->offset.y * scale;

            //Position
            vertices[nVertex + 0].position = glm::vec3(x + offX,          y - offY,           layer);
            vertices[nVertex + 1].position = glm::vec3(x + offX + cWidth, y - offY,           layer);
            vertices[nVertex + 2].position = glm::vec3(x + offX + cWidth, y - offY - cHeight, layer);
            vertices[nVertex + 3].position = glm::vec3(x + offX,          y - offY - cHeight, layer);

            // set Texture Coords
            vertices[nVertex + 0].textureCoords = glm::vec2(g->pos.x,               1 - (g->pos.y));
            vertices[nVertex + 1].textureCoords = glm::vec2(g->pos.x + g->dim.x,    1 - (g->pos.y));
            vertices[nVertex + 2].textureCoords = glm::vec2(g->pos.x + g->dim.x,    1 - (g->pos.y + g->dim.y));
            vertices[nVertex + 3].textureCoords = glm::vec2(g->pos.x,               1 - (g->pos.y + g->dim.y));

            indices[indexCnt + 0] = nVertex + 0;
            indices[indexCnt + 1] = nVertex + 1;
            indices[indexCnt + 2] = nVertex + 2;
            indices[indexCnt + 3] = nVertex + 0;
            indices[indexCnt + 4] = nVertex + 2;
            indices[indexCnt + 5] = nVertex + 3;

            nVertex+=4;
            indexCnt+=6;

            x += cWidth + whitespace * options.spacing.x;
        }

        std::vector<IVertex*> iVertices(vertices.size());
        for (uint32_t i = 0; i < vertices.size(); i++) {
            vertices[i].color = options.color;
            iVertices[i] = &vertices[i];
            iVertices[i]->texture = (float)font->getID();
        }
        autoBatch<DefaultVertex>(iVertices, getRenderPrio(layer), &indices, opt);
    }

    template<class T>
    void StdRenderer::autoBatch(std::vector<IVertex *> vertices, uint16_t prio, std::vector<GLuint> *indices, Options* opt) {
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

                // Set Pre/Post Render
                if(opt){
                    _auto_batches[id]->addPreRender(opt->preRender, opt->preRenderData);
                    _auto_batches[id]->addPostRender(opt->postRender, opt->postRenderData);
                }

                // Update start and position
                start = stop + 1;
                textureID = v->texture;
            }

            stop++;
        }

        if(opt && opt->autoFree)
            delete opt;
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

    void StdRenderer::addPreRender(void (*func)(void *, IStdRenderer *, double), void *o) {
        preRender = func;
        _o_pre_render = o;
    }

    void StdRenderer::addPostRender(void (*func)(void *, IStdRenderer *, double), void *o) {
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
        _width = width;

        if(!_deferred)
            glfwSetWindowSize(_window, width, _height);
        else{
            delete _deferred_txt;
            _deferred_txt = new Texture2D(_width, _height, nullptr);
        }
    }

    int StdRenderer::getWidth() {
        if(!_deferred)
            glfwGetWindowSize(_window, &_width, &_height);

        return _width;
    }

    void StdRenderer::setHeight(int height) {
        _height = height;

        if(!_deferred)
            glfwSetWindowSize(_window, _width, height);
        else{
            delete _deferred_txt;
            _deferred_txt = new Texture2D(_width, _height, nullptr);
        }
    }

    int StdRenderer::getHeight() {
        if(!_deferred)
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
        // TODO: Fix memory leak
        //delete this;

        if(_deferred){
            glDeleteBuffers(1, &_deferred_fbo);
            delete _deferred_txt;
        }
    }

    Font *StdRenderer::getFont(const std::string &name) {
        if(!_fonts.count(name)){
            kikanPrintE("[ERROR] Font %s not found\n", name.c_str());
            return _fonts["default"];
        }

        return _fonts[name];
    }

    void StdRenderer::addFont(Font *font, const std::string &name) {
        _fonts[name] = font;
    }

    uint16_t StdRenderer::getRenderPrio(float layer) {
        return (uint16_t)(layer * -100.f + 32768);
    }

    Texture2D *StdRenderer::getDeferredTxt() {
        return _deferred_txt;
    }
}