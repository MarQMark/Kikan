#ifndef KIKAN_STD_RENDERER_H
#define KIKAN_STD_RENDERER_H

#include <map>
#include <string>
#include <cstring>
#include "glm/glm.hpp"
#include "Shader.h"
#include "Kikan/renderer/IStdRenderer.h"
#include "Kikan/renderer/stdRenderer/batching/AutoBatch.h"
#include "Kikan/renderer/stdRenderer/batching/ManuelBatch.h"
#include "Kikan/renderer/stdRenderer/vertices/DefaultVertex.h"
#include "Kikan/renderer/stdRenderer/vertices/VertexRegistry.h"
#include "Font.h"

namespace Kikan {
class StdRenderer : public IStdRenderer {
    public:
        struct InitParams {
            int width = 1280;
            int height = 720;
            bool deferred = false;
        };

        explicit StdRenderer(void* params){
            auto initParams = (struct InitParams*)params;

            bool defaultParams = false;
            if(!initParams) {
                defaultParams = true;
                initParams = new InitParams;
            }

            _width = initParams->width;
            _height = initParams->height;
            _deferred = initParams->deferred;

            if(defaultParams)
                delete initParams;

            setup_openGl();
        }

        void destroy() override;

        GLFWwindow* getWindow() override;
        void setWidth(int width) override;
        int getWidth() override;
        void setHeight(int height) override;
        int getHeight() override;

        glm::mat4x4 mvp{};

        /*
        *  Uses Auto-batching with DefaultVertex.
        */
        void renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer, Options* opt = nullptr) override;

        /*
        *  Uses Auto-batching with DefaultVertex.
         *
         *  Order of Points is:
        *      1 ------ 2
        *      |        |
        *      |        |
        *      4 ------ 3
        */
        void renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer, Options* opt = nullptr) override;

        /*
        *  Uses Auto-batching with DefaultVertex.
        *
        *  Uses Ear-Clipping-Algorithm to divide into Triangles.
        *  This means Polygons cannot intersect with themselves, cannot have holes and
        *  three or more vertices cannot form a line
        */
        void renderPolygon(std::vector<glm::vec2>& points, glm::vec4 color, float layer, Options* opt = nullptr) override;

        /*
        * Uses Auto-batching with DefaultVertex.
        *
        * Texture ID of IVertex gets used for binding correct Texture.
        * In Vertex data textureID is always 0 since AutoBatching only allows one Texture per Batch
        *
        * Order of Points is:
        *      1 ------ 2
        *      |        |
        *      |        |
        *      4 ------ 3
        */
        void renderTexture2D(glm::vec2 p[4], glm::vec2 texCoords[4], GLuint textureId, glm::vec4 color = glm::vec4(0), float layer = 0, Options* opt = nullptr) override;

        /*
        * Uses Auto-batching with DefaultVertex.
        *
        * If not font is specified the default font is used
        */
        void renderText(std::string text, glm::vec2 pos, float height, float layer = 0, Font::Options options = {}, Options* opt = nullptr) override;

        /*
        *  Each Auto-Batch gets an unique ID: 2 Bytes Priority + 4 Bytes Vertex Signature + 2 Bytes Texture ID
        *
        * prio: The priority given in accordance to render order. The lower, the earlier it gets rendered
        *       For each new prio a new batch gets created
        */
        template <class T>
        void autoBatch(std::vector<IVertex*> vertices, uint16_t prio = 0, std::vector<GLuint>* indices = nullptr, Options* opt = nullptr);

        uint16_t getRenderPrio(float layer) override;

        void addBatch(ManuelBatch* batch, unsigned int key) override;
        ManuelBatch* getBatch(unsigned int key) override;

        void render(double dt) override;
        void addPreRender(void (*func)(void* o, IStdRenderer* renderer, double dt), void* o) override;
        void addPostRender(void (*func)(void* o, IStdRenderer* renderer, double dt), void* o) override;
        void overrideRender(Override* ovr) override;

        Shader* shader(const std::string& name = "default") override;
        void shader(Shader* shader, const std::string& name = "default") override;

        Font* getFont(const std::string& name = "default") override;
        void addFont(Font* font, const std::string& name = "default") override;

        static void queryErrors(const std::string& tag);
    private:
        ~StdRenderer(){
            glfwTerminate();
        }

        GLFWwindow *_window = nullptr;

        int _width;
        int _height;

        std::map<std::string, Shader*> _shaders;
        std::map<unsigned int, ManuelBatch*> _batches;

        std::map<uint64_t, AutoBatch*> _auto_batches;

        std::map<std::string, Font*> _fonts;

        void (*preRender)(void* o, IStdRenderer* renderer, double dt) = nullptr;
        void (*postRender)(void* o, IStdRenderer* renderer, double dt) = nullptr;
        void* _o_pre_render = nullptr;
        void* _o_post_render = nullptr;
        Override* _override_render = nullptr;

        bool _deferred = false;
        GLuint _deferred_fbo = 0;
        Texture2D* _deferred_txt = {};
        Texture2D* getDeferredTxt();

        void setup_openGl();
        static uint64_t auto_batch_id(uint32_t signature, uint16_t prio, float textureID);
    };
}

#endif //KIKAN_STD_RENDERER_H
