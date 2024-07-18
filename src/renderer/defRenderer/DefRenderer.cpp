#include "Kikan/renderer/defRenderer/DefRenderer.h"

namespace Kikan {

    DefRenderer::DefRenderer(IStdRenderer *renderer) {
        _renderer = renderer;
    }

    void DefRenderer::render(double dt) {
        if(_renderer)
            _renderer->render(dt);
    }

    void DefRenderer::destroy() {
        if(_renderer)
            _renderer->destroy();
    }

    GLFWwindow *DefRenderer::getWindow() {
        if(_renderer)
            _renderer->getWindow();

        return nullptr;
    }

    void DefRenderer::setWidth(int width) {
        if(_renderer)
            _renderer->setWidth(width);
    }

    int DefRenderer::getWidth() {
        if(_renderer)
            _renderer->getWidth();

        return 0;
    }

    void DefRenderer::setHeight(int height) {
        if(_renderer)
            _renderer->setHeight(height);
    }

    int DefRenderer::getHeight() {
        if(_renderer)
            _renderer->getHeight();

        return 0;
    }

    void DefRenderer::renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, float layer,
                                     IStdRenderer::Options *opt) {
        if(_renderer)
            _renderer->renderTriangle(p1, p2, p3, color, layer, opt);
    }

    void DefRenderer::renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, float layer,
                                 IStdRenderer::Options *opt) {
        if(_renderer)
            _renderer->renderQuad(p1, p2, p3, p4, color, layer, opt);
    }

    void DefRenderer::renderPolygon(std::vector<glm::vec2> &points, glm::vec4 color, float layer,
                                    IStdRenderer::Options *opt) {
        if(_renderer)
            _renderer->renderPolygon(points, color, layer, opt);
    }

    void
    DefRenderer::renderTexture2D(glm::vec2 *p, glm::vec2 *texCoords, GLuint textureId, glm::vec4 color, float layer,
                                 IStdRenderer::Options *opt) {
        if(_renderer)
            _renderer->renderTexture2D(p, texCoords, textureId, color, layer, opt);
    }

    void DefRenderer::renderText(std::string text, glm::vec2 pos, float height, float layer, Font::Options options,
                                 IStdRenderer::Options *opt) {
        if(_renderer)
            _renderer->renderText(text, pos, height, layer, options, opt);
    }

    uint16_t DefRenderer::getRenderPrio(float layer) {
        if(_renderer)
            _renderer->getRenderPrio(layer);

        return 0;
    }

    void DefRenderer::addBatch(ManuelBatch *batch, unsigned int key) {
        if(_renderer)
            _renderer->addBatch(batch, key);
    }

    ManuelBatch *DefRenderer::getBatch(unsigned int key) {
        if(_renderer)
            _renderer->getBatch(key);

        return nullptr;
    }

    void DefRenderer::addPreRender(void (*func)(void *, IStdRenderer *, double), void *o) {
        if(_renderer)
            _renderer->addPreRender(func, o);
    }

    void DefRenderer::addPostRender(void (*func)(void *, IStdRenderer *, double), void *o) {
        if(_renderer)
            _renderer->addPostRender(func, o);
    }

    void DefRenderer::overrideRender(IStdRenderer::Override *ovr) {
        if(_renderer)
            _renderer->overrideRender(ovr);
    }

    Shader *DefRenderer::shader(const std::string &name) {
        if(_renderer)
            _renderer->shader(name);

        return nullptr;
    }

    void DefRenderer::shader(Shader *shader, const std::string &name) {
        if(_renderer)
            _renderer->shader(shader, name);
    }

    Font *DefRenderer::getFont(const std::string &name) {
        if(_renderer)
            _renderer->getFont(name);

        return nullptr;
    }

    void DefRenderer::addFont(Font *font, const std::string &name) {
        if(_renderer)
            _renderer->addFont(font, name);
    }
}