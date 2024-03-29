#include <utility>

#include "Kikan/ui/elements/Label.h"
#include "Kikan/Engine.h"

namespace Kikan{

    Label::Label(std::string name, glm::vec2 pos, glm::vec2 dim, std::string text) : IUIElement(std::move(name)), _text(std::move(text)){
        init(pos, dim);
    }

    Label::Label(std::string name, glm::vec2 pos, glm::vec2 dim, Texture2D *txt) : IUIElement(std::move(name)), _txt(txt) {
        init(pos, dim);
    }

    void Label::init(glm::vec2 pos, glm::vec2 dim) {
        this->pos = pos;
        this->dim = dim;

        _font_options.font = ((StdRenderer*)Engine::Kikan()->getRenderer())->getFont();
        _font_options.color = glm::vec4(1);
        _font_size = dim.y;
        _text_offset = glm::vec2(0, -dim.y * .1);
    }

    void Label::render(glm::vec2 parentPos) {
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        float renderLayer = Engine::Kikan()->getUI()->renderLayer;

        if(_txt){
            glm::vec2 position[4];
            position[0] = parentPos + _txt_pos[0] = glm::vec2(pos.x,          pos.y);
            position[1] = parentPos + _txt_pos[1] = glm::vec2(pos.x + dim.x,  pos.y);
            position[2] = parentPos + _txt_pos[2] = glm::vec2(pos.x + dim.x,  pos.y - dim.y);
            position[3] = parentPos + _txt_pos[3] = glm::vec2(pos.x,          pos.y - dim.y);
            renderer->renderTexture2D(position, _txt_coords, _txt->get(), _bg_color, renderLayer + _txt_layer_offset, &_opt);
        }
        else if(_bg_color.w != 0)
            renderer->renderQuad(
                    glm::vec2(parentPos.x + pos.x,          parentPos.y + pos.y),
                    glm::vec2(parentPos.x + pos.x + dim.x,  parentPos.y + pos.y),
                    glm::vec2(parentPos.x + pos.x + dim.x,  parentPos.y + pos.y - dim.y),
                    glm::vec2(parentPos.x + pos.x,          parentPos.y + pos.y - dim.y),
                    _bg_color,
                    renderLayer + _bg_layer_offset,
                    &_opt);

        if(!_text.empty())
            renderer->renderText(_text, pos + parentPos, _font_size, renderLayer + _font_layer_offset, _font_options, &_text_opt);
    }

    void Label::setBgLayerOffset(float offset) {
        _bg_layer_offset = offset;
    }

    float Label::getBgLayerOffset() const {
        return _bg_layer_offset;
    }

    void Label::setBgColor(glm::vec4 color) {
        _bg_color = color;
    }

    glm::vec4 Label::getBgColor() {
        return _bg_color;
    }

    void Label::setText(std::string text) {
        _text = std::move(text);
    }

    std::string Label::getText() {
        return _text;
    }

    void Label::setFontOptions(Font::Options options) {
        _font_options = options;
    }

    Font::Options Label::getFontOptions() {
        return _font_options;
    }

    void Label::setFontSize(float size) {
        _font_size = size;
    }

    float Label::getFontSize() const {
        return _font_size;
    }

    void Label::setFontLayerOffset(float offset) {
        _font_layer_offset = offset;
    }

    float Label::getFontLayerOffset() const {
        return _font_layer_offset;
    }

    void Label::setTexture2D(Texture2D *txt) {
        _txt = txt;
    }

    Texture2D *Label::getTexture() {
        return _txt;
    }

    void Label::setTextureCoords(glm::vec2 *txtCoords) {
        for(int i = 0; i < 4; i++)
            _txt_coords[i] = txtCoords[i];
    }

    glm::vec2* Label::getTextureCoords() {
        return _txt_coords;
    }

    void Label::setTextureLayerOffset(float offset) {
        _txt_layer_offset = offset;
    }

    float Label::getTextureLayerOffset() const {
        return _txt_layer_offset;
    }

    void Label::destroy() {
        delete this;
    }

    void Label::setTextOffset(glm::vec2 offset) {
        _text_offset = offset;
    }

    glm::vec2 Label::getTextOffset() {
        return _text_offset;
    }
}



