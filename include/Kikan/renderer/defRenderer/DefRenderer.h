#ifndef KIKAN_DEFRENDERER_H
#define KIKAN_DEFRENDERER_H

#include "GL/glew.h"
#include "Kikan/renderer/IStdRenderer.h"

namespace Kikan{

    class DefRenderer : public IStdRenderer{

    public:
        explicit DefRenderer(IStdRenderer* renderer);

        void render(double dt) override;
        void destroy() override;

        GLFWwindow* getWindow() override;

        void setWidth(int width) override;
        int getWidth() override;
        void setHeight(int height) override;
        int getHeight() override;

        void renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer, Options* opt = nullptr) override;
        void renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer, Options* opt = nullptr) override;
        void renderPolygon(std::vector<glm::vec2>& points, glm::vec4 color, float layer, Options* opt = nullptr) override;
        void renderTexture2D(glm::vec2 p[4], glm::vec2 texCoords[4], GLuint textureId, glm::vec4 color = glm::vec4(0), float layer = 0, Options* opt = nullptr) override;
        void renderText(std::string text, glm::vec2 pos, float height, float layer = 0, Font::Options options = {}, Options* opt = nullptr) override;

        uint16_t getRenderPrio(float layer) override;

        void addBatch(ManuelBatch* batch, unsigned int key) override;
        ManuelBatch* getBatch(unsigned int key) override;

        void addPreRender(void (*func)(void* o, IStdRenderer* renderer, double dt), void* o) override;
        void addPostRender(void (*func)(void* o, IStdRenderer* renderer, double dt), void* o) override;
        void overrideRender(Override* ovr) override;

        Shader* shader(const std::string& name = "default") override;
        void shader(Shader* shader, const std::string& name = "default") override;

        Font* getFont(const std::string& name = "default") override;
        void addFont(Font* font, const std::string& name = "default") override;

    private:

        IStdRenderer* _renderer = {};
    };
}

#endif //KIKAN_DEFRENDERER_H
