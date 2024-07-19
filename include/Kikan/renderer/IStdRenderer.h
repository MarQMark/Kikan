#ifndef KIKAN_ISTDRENDERER_H
#define KIKAN_ISTDRENDERER_H

#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>
#include <string>

#include "stdRenderer/Font.h"
#include "stdRenderer/Shader.h"
#include "stdRenderer/batching/AutoBatch.h"
#include "stdRenderer/batching/ManuelBatch.h"

namespace Kikan {
    class IStdRenderer : public Renderer {

    public:
        class Override{
        public:
            virtual void preRender(IStdRenderer* renderer, double dt) = 0;
            virtual void postRender(IStdRenderer* renderer, double dt) = 0;
        };

        struct Options{
            void(*preRender)(AutoBatch*, void*) = nullptr;
            void(*postRender)(AutoBatch*, void*) = nullptr;
            void* preRenderData = nullptr;
            void* postRenderData = nullptr;

            bool autoFree = false;
        };

        virtual GLFWwindow* getWindow() = 0;

        virtual void setWidth(int width) = 0;
        virtual int getWidth() = 0;
        virtual void setHeight(int height) = 0;
        virtual int getHeight() = 0;

        virtual void renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer, Options* opt = nullptr) = 0;
        virtual void renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer, Options* opt = nullptr) = 0;
        virtual void renderPolygon(std::vector<glm::vec2>& points, glm::vec4 color, float layer, Options* opt = nullptr) = 0;
        virtual void renderTexture2D(glm::vec2 p[4], glm::vec2 texCoords[4], GLuint textureId, glm::vec4 color = glm::vec4(0), float layer = 0, Options* opt = nullptr) = 0;
        virtual void renderText(std::string text, glm::vec2 pos, float height, float layer = 0, Font::Options options = {}, Options* opt = nullptr) = 0;

        virtual uint16_t getRenderPrio(float layer) = 0;

        virtual void addBatch(ManuelBatch* batch, unsigned int key) = 0;
        virtual ManuelBatch* getBatch(unsigned int key) = 0;

        virtual void addPreRender(void (*func)(void* o, IStdRenderer* renderer, double dt), void* o) = 0;
        virtual void addPostRender(void (*func)(void* o, IStdRenderer* renderer, double dt), void* o) = 0;
        virtual void overrideRender(Override* ovr) = 0;

        virtual Shader* shader(const std::string& name = "default") = 0;
        virtual void shader(Shader* shader, const std::string& name = "default") = 0;

        virtual Font* getFont(const std::string& name = "default") = 0;
        virtual void addFont(Font* font, const std::string& name = "default") = 0;

        virtual bool shouldClose() = 0;
        virtual void setTitle(const char* title) = 0;
    };
}

#endif //KIKAN_ISTDRENDERER_H
