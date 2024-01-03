#include "Kikan/ui/elements/Button.h"
#include "Kikan/Engine.h"

namespace Kikan {
    Button::Button(glm::vec2 pos, glm::vec2 dim) : IInteractable() {
        this->pos = pos;
        this->dim = dim;
        adjust_colors();
    }

    void Button::render() {
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        float renderLayer = Engine::Kikan()->getUI()->renderLayer;

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

        renderer->renderQuad(
                glm::vec2(pos.x,          pos.y),
                glm::vec2(pos.x + dim.x,  pos.y),
                glm::vec2(pos.x + dim.x,  pos.y - dim.y),
                glm::vec2(pos.x,          pos.y - dim.y),
                color,
                renderLayer + _layer_offset,
                &_opt);
    }

    void Button::adjust_colors() {
        if(!_custom_hover_color)
            _hover_color = _base_color + (glm::vec4(1.,1.,1.,0) * .1f);

        if(!_custom_hold_color)
            _hold_color = _base_color - (glm::vec4(1.,1.,1.,0) * .2f);

        if(!_custom_disabled_color)
            _disabled_color = _base_color + (glm::vec4(1.,1.,1.,0) * .5f);
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
        _custom_disabled_color = true;
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
}