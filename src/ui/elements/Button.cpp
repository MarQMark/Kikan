#include "Kikan/ui/elements/Button.h"
#include "Kikan/Engine.h"

namespace Kikan {
    Button::Button(std::string name, glm::vec2 pos, glm::vec2 dim, std::string text) : IInteractable(std::move(name)) {
        this->pos = pos;
        this->dim = dim;
        _focus_border = Engine::Kikan()->getUI()->getHeight() / 200.f;
        _disabled_color = glm::vec4(.4,.4,.4,1);
        adjust_colors();

        _text = std::move(text);
        _font_size = dim.y * .8;
        _text_offset = glm::vec2(0, -dim.y * .1);
        _font_options.font = ((StdRenderer*)Engine::Kikan()->getRenderer())->getFont();
        _font_options.color = glm::vec4(1);
    }

    void Button::render(glm::vec2 parentPos) {
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        float renderLayer = Engine::Kikan()->getUI()->renderLayer;
        glm::vec2 position = pos + parentPos;

        glm::vec4 color;
        switch (getState()) {
            case IInteractable::NONE:
                color = _base_color;
                break;
            case IInteractable::HOVERED:
                color = _hover_color;
                break;
            case IInteractable::HELD:
                color = _hold_color;
                break;
            default:
                color = _base_color;
                break;
        }
        if(!interactable)
            color = _disabled_color;

        renderer->renderQuad(
                glm::vec2(position.x,          position.y),
                glm::vec2(position.x + dim.x,  position.y),
                glm::vec2(position.x + dim.x,  position.y - dim.y),
                glm::vec2(position.x,          position.y - dim.y),
                color,
                renderLayer + _layer_offset,
                &_opt);

        if(!_text.empty())
            renderer->renderText(_text, pos + parentPos + _text_offset, _font_size, renderLayer + _layer_offset + _font_layer_offset, _font_options, &_text_opt);

        if(focused){
            float thickness = _focus_border;
            renderer->renderQuad(
                    glm::vec2(pos),
                    glm::vec2(pos) + glm::vec2(dim.x,   0),
                    glm::vec2(pos) + glm::vec2(dim.x,   0) + glm::vec2(0, -thickness),
                    glm::vec2(pos) + glm::vec2(0,       -thickness),
                    glm::vec4(.2,.2,.2,.5),
                    renderLayer + _layer_offset - 0.01f,
                    &_opt);
            renderer->renderQuad(
                    glm::vec2(pos) + glm::vec2(dim.x, 0)        + glm::vec2(-thickness, -thickness),
                    glm::vec2(pos) + glm::vec2(dim.x, 0)        + glm::vec2(0,          -thickness),
                    glm::vec2(pos) + glm::vec2(dim.x, -dim.y)  + glm::vec2(0,          thickness),
                    glm::vec2(pos) + glm::vec2(dim.x, -dim.y)  + glm::vec2(-thickness, thickness),
                    glm::vec4(.2,.2,.2,.5),
                    renderLayer + _layer_offset - 0.01f,
                    &_opt);
            renderer->renderQuad(
                    glm::vec2(pos) + glm::vec2(0,       -dim.y) + glm::vec2(0, thickness),
                    glm::vec2(pos) + glm::vec2(dim.x,   -dim.y) + glm::vec2(0, thickness),
                    glm::vec2(pos) + glm::vec2(dim.x,   -dim.y),
                    glm::vec2(pos) + glm::vec2(0,       -dim.y),
                    glm::vec4(.2,.2,.2,.5),
                    renderLayer + _layer_offset - 0.01f,
                    &_opt);
            renderer->renderQuad(
                    glm::vec2(pos) + glm::vec2(0,           -thickness),
                    glm::vec2(pos) + glm::vec2(thickness,   -thickness),
                    glm::vec2(pos) + glm::vec2(0,           -dim.y)    + glm::vec2(thickness, thickness),
                    glm::vec2(pos) + glm::vec2(0,           -dim.y)    + glm::vec2(0,         thickness),
                    glm::vec4(.2,.2,.2,.5),
                    renderLayer + _layer_offset - 0.01f,
                    &_opt);
        }
    }

    void Button::adjust_colors() {
        if(!_custom_hover_color)
            _hover_color = _base_color + (glm::vec4(1.,1.,1.,0) * .1f);

        if(!_custom_hold_color)
            _hold_color = _base_color - (glm::vec4(1.,1.,1.,0) * .2f);
    }

    void Button::setColor(glm::vec4 color) {
        _base_color = color;
        adjust_colors();
    }
    void Button::setHoverColor(glm::vec4 color) {
        _hover_color = color;
        _custom_hover_color = true;
    }
    void Button::setHoldColor(glm::vec4 color) {
        _hold_color = color;
        _custom_hold_color = true;
    }
    void Button::setDisabledColor(glm::vec4 color) {
        _disabled_color = color;
    }
    glm::vec4 Button::getColor() {
        return _base_color;
    }
    glm::vec4 Button::getHoverColor() {
        return _hover_color;
    }
    glm::vec4 Button::getHoldColor() {
        return _hold_color;
    }
    glm::vec4 Button::getDisabledColor() {
        return _disabled_color;
    }
    void Button::setLayerOffset(float offset) {
        _layer_offset = offset;
    }
    float Button::getLayerOffset() {
        return _layer_offset;
    }

    void Button::destroy() {
        delete this;
    }

    void Button::setText(std::string text) {
        _text = std::move(text);
    }

    std::string Button::getText() {
        return _text;
    }

    void Button::setFontOptions(Font::Options options) {
        _font_options = options;
    }

    Font::Options Button::getFontOptions() {
        return _font_options;
    }

    void Button::setFontSize(float size) {
        _font_size = size;
    }

    float Button::getFontSize() const {
        return _font_size;
    }

    void Button::setTextOffset(glm::vec2 offset) {
        _text_offset = offset;
    }

    glm::vec2 Button::getTextOffset() {
        return _text_offset;
    }
}