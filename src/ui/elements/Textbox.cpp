#include <utility>
#include <algorithm>

#include "Kikan/ui/elements/Textbox.h"
#include "Kikan/Engine.h"

namespace Kikan{

    void textboxOnClick(IInteractable* interactable, void* data){
        auto* textbox = (Textbox*)interactable;
        glm::vec2 mouse = Engine::Kikan()->getUI()->getUIMousePos();
        // TODO: include parent nodes in pos
        textbox->setCursor(mouse.x - textbox->pos.x);
    }

    Textbox::Textbox(std::string name, glm::vec2 pos, glm::vec2 dim) : IInteractable(std::move(name)) {
        this->pos = pos;
        this->dim = dim;

        _font_options.font = ((StdRenderer*)Engine::Kikan()->getRenderer())->getFont();
        _font_size = dim.y * 0.9f * 0.5f;
        _text_offset.y = -dim.y * 0.2f;

        _cursor_style.dim.y = dim.y * 0.9f * .6f;
        _cursor_style.dim.x = .5;
        _cursor_style.off.y = -dim.y * (1 - 0.9f * .75f) / 2.f;

        registerCallback(textboxOnClick, State::PRESSED);
    }

    void Textbox::render(glm::vec2 parentPos) {
        update_font_scale();
        _cursor_style.dim.x = _whitespace * _font_options.spacing.x;
        _cursor_style.off.x = (_whitespace * _font_options.spacing.x) / 2.f;
        _text_offset.x = _whitespace * _font_options.spacing.x;


        calc_bounds();
        std::string sub = _text.substr(_text_bound_l, _text_bound_r - _text_bound_l);

        render_text(sub, pos + parentPos + _text_offset);
        render_outline();

        if(!focused || _blink_time > _blink_max_time)
            render_cursor();
    }

    void Textbox::update() {
        if(!focused)
            return;

        auto* input = Engine::Kikan()->getInput();
        if(_left && !input->keyPressed(Key::LEFT))
            setCursor(_cursor - 1);
        if(_right && !input->keyPressed(Key::RIGHT))
            setCursor(_cursor + 1);

        _left = input->keyPressed(Key::LEFT);
        _right = input->keyPressed(Key::RIGHT);

        _blink_time -= Engine::Kikan()->time.dt;
        if(_blink_time < 0)
            reset_blink();
    }

    void Textbox::setCursor(int32_t cursor) {
        cursor = std::max(std::min(cursor, (int32_t)_text.size()), 0);
        if(cursor >= (int32_t)_text_bound_r){
            _left_bound = false;
            _text_bound_r = cursor;
        }
        else if(cursor <= (int32_t)_text_bound_l){
            _left_bound = true;
            _text_bound_l = cursor;
        }
        _cursor = cursor;
        reset_blink();
    }

    void Textbox::setCursor(float offset) {
        std::string sub = _text.substr(_text_bound_l, _text_bound_r - _text_bound_l);
        float w = 0;
        for(uint32_t i = 0; i < sub.size(); i++){
            char c = sub[i];
            float cWidth = get_char_len(c);
            if(i == 0 && !_left_bound)
                cWidth *= (1 - _cut_percentage);

            w += cWidth;
            if(w >= offset){
                if(w - offset < cWidth / 2.f)
                    setCursor((int32_t)(_text_bound_l + i + 1));
                else
                    setCursor((int32_t)(_text_bound_l + i));
                break;
            }
        }
    }

    void Textbox::setText(std::string text) {
        _text = std::move(text);
        setCursor((int32_t)_text.size());
    }

    std::string Textbox::getText() {
        return _text;
    }

    void Textbox::destroy() {
        delete this;
    }

    void Textbox::calc_bounds() {
        _cut_percentage = 1;
        float textWidth = 0;
        int32_t i = _left_bound ? (int32_t)_text_bound_l : (int32_t)_text_bound_r - 1;
        for(;;){
            char c = _text[i];
            float cWidth = 0;
            if(c == ' ')       { cWidth = _whitespace;    }
            else if(c == '\t') { cWidth = _whitespace * 4;}
            else if(c == '\r') { cWidth = 0;          }
            else if(c == '\n') { cWidth = 0;          }
            else {
                Font::Glyph* g = _font_options.font->getGlyph(c);
                if(!g) continue;
                cWidth = (g->dim.x * _font_scale) + (g->offset.x * _font_scale);
            }

            float spacing = c < 21 ? 0 : _whitespace * _font_options.spacing.x;

            if(textWidth + cWidth >= (dim.x - 2 * _text_offset.x)){
                _cut_percentage = ((dim.x - 2 * _text_offset.x) - (textWidth + spacing))/cWidth;
                if(_left_bound)
                    _text_bound_r = i + 1;
                else
                    _text_bound_l = i;

                break;
            }
            textWidth += cWidth + spacing;


            if(_left_bound){
                i++;
                if(i > (int32_t)_text.size() - 1)
                    break;
            }
            else{
                i--;
                if(i < 0)
                    break;
            }
        }
    }

    void Textbox::render_text(const std::string& text, glm::vec2 pos) {
        float layer = Engine::Kikan()->getUI()->renderLayer + _layer_offset;
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();

        std::vector<DefaultVertex> vertices(4 * text.size());
        std::vector<GLuint> indices(6 * text.size());

        GLuint indexCnt = 0;
        uint32_t nVertex = 0;
        float y = pos.y;
        float x = pos.x;

        for(uint32_t i = 0; i < text.size(); i++){

            float yScale = 1;
            if((_left_bound && i == text.size() - 1) || (!_left_bound && i == 0))
                yScale = _cut_percentage;

            char c = text[i];
            if(c == ' ')       { x += _whitespace * yScale;      continue; }
            else if(c == '\t') { x += _whitespace * yScale * 4;  continue; }
            else if(c == '\r') { x = pos.x;                      continue; }
            else if(c == '\n') {
                x = pos.x;
                y-= _font_size * 1.5f * _font_options.spacing.y;
                continue;
            }

            Font::Glyph* g = _font_options.font->getGlyph(c);
            if(!g) continue;
            float cWidth = g->dim.x  * _font_scale;
            float cHeight = g->dim.y * _font_scale;
            float offX = g->offset.x * _font_scale;
            float offY = g->offset.y * _font_scale;

            float posCutL = 0;
            float posCutR = 0;
            float texCutL = 0;
            float texCutR = 0;
            if(_left_bound && i == text.size() - 1){
                posCutR = -cWidth * (1 - _cut_percentage);
                texCutR = -g->dim.x * (1 - _cut_percentage);
            }
            else if(!_left_bound && i == 0){
                posCutR = -cWidth * (1 - _cut_percentage);
                texCutL = g->dim.x * (1 - _cut_percentage);
            }

            //Position
            vertices[nVertex + 0].position = glm::vec3(x + offX          + posCutL,  y - offY,           layer);
            vertices[nVertex + 1].position = glm::vec3(x + offX + cWidth + posCutR,  y - offY,           layer);
            vertices[nVertex + 2].position = glm::vec3(x + offX + cWidth + posCutR,  y - offY - cHeight, layer);
            vertices[nVertex + 3].position = glm::vec3(x + offX          + posCutL,  y - offY - cHeight, layer);

            // set Texture Coords
            vertices[nVertex + 0].textureCoords = glm::vec2(g->pos.x            + texCutL,      1 - (g->pos.y));
            vertices[nVertex + 1].textureCoords = glm::vec2(g->pos.x + g->dim.x + texCutR,      1 - (g->pos.y));
            vertices[nVertex + 2].textureCoords = glm::vec2(g->pos.x + g->dim.x + texCutR,      1 - (g->pos.y + g->dim.y));
            vertices[nVertex + 3].textureCoords = glm::vec2(g->pos.x            + texCutL,      1 - (g->pos.y + g->dim.y));

            indices[indexCnt + 0] = nVertex + 0;
            indices[indexCnt + 1] = nVertex + 1;
            indices[indexCnt + 2] = nVertex + 2;
            indices[indexCnt + 3] = nVertex + 0;
            indices[indexCnt + 4] = nVertex + 2;
            indices[indexCnt + 5] = nVertex + 3;

            nVertex+=4;
            indexCnt+=6;

            x += (cWidth * yScale) + (_whitespace * _font_options.spacing.x);
        }

        std::vector<IVertex*> iVertices(vertices.size());
        for (uint32_t i = 0; i < vertices.size(); i++) {
            vertices[i].color = glm::vec4(0);//options.color; // TODO: Fix Font coloring
            iVertices[i] = &vertices[i];
            iVertices[i]->texture = (float)_font_options.font->getID();
        }
        renderer->autoBatch<DefaultVertex>(iVertices, renderer->getRenderPrio(layer), &indices, &_opt);
    }

    void Textbox::render_outline() {
        float layer = Engine::Kikan()->getUI()->renderLayer + _layer_offset;
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        float thickness = .2;
        renderer->renderQuad(
                glm::vec2(pos),
                glm::vec2(pos) + glm::vec2(dim.x,   0),
                glm::vec2(pos) + glm::vec2(dim.x,   0) + glm::vec2(0, -thickness),
                glm::vec2(pos) + glm::vec2(0,       -thickness),
                glm::vec4(.6,.6,.6,.5),
                layer - 0.01f,
                &_opt);
        renderer->renderQuad(
                glm::vec2(pos) + glm::vec2(dim.x, 0)        + glm::vec2(-thickness, -thickness),
                glm::vec2(pos) + glm::vec2(dim.x, 0)        + glm::vec2(0,          -thickness),
                glm::vec2(pos) + glm::vec2(dim.x, -dim.y)   + glm::vec2(0,          thickness),
                glm::vec2(pos) + glm::vec2(dim.x, -dim.y)   + glm::vec2(-thickness, thickness),
                glm::vec4(.6,.6,.6,.5),
                layer - 0.01f,
                &_opt);
        renderer->renderQuad(
                glm::vec2(pos) + glm::vec2(0,       -dim.y) + glm::vec2(0, thickness),
                glm::vec2(pos) + glm::vec2(dim.x,   -dim.y) + glm::vec2(0, thickness),
                glm::vec2(pos) + glm::vec2(dim.x,   -dim.y),
                glm::vec2(pos) + glm::vec2(0,       -dim.y),
                glm::vec4(.6,.6,.6,.5),
                layer - 0.01f,
                &_opt);
        renderer->renderQuad(
                glm::vec2(pos) + glm::vec2(0,           -thickness),
                glm::vec2(pos) + glm::vec2(thickness,   -thickness),
                glm::vec2(pos) + glm::vec2(0,           -dim.y)    + glm::vec2(thickness, thickness),
                glm::vec2(pos) + glm::vec2(0,           -dim.y)    + glm::vec2(0,         thickness),
                glm::vec4(.6,.6,.6,.5),
                layer - 0.01f,
                &_opt);
    }

    void Textbox::render_cursor() {
        float layer = Engine::Kikan()->getUI()->renderLayer + _layer_offset - 0.1f;
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();

        float xOff= 0;
        if(_cursor != 0){
            if(_left_bound){
                std::string sub = _text.substr(_text_bound_l, _cursor - _text_bound_l);
                xOff = get_text_len(sub);
                if(_cursor > 1 && _text[_cursor - 1] == ' ')
                    xOff -= _whitespace * .2f;
                else
                    xOff -= _whitespace * _font_options.spacing.x * .5f;
            }
            else{
                std::string sub = _text.substr(_cursor, _text_bound_r - _cursor);
                xOff = (dim.x - 2 * _text_offset.x) - get_text_len(sub);
                if(_cursor > 1 && _text[_cursor - 1] == ' ')
                    xOff -= _whitespace * .2f;
                else
                    xOff -= _whitespace * _font_options.spacing.x * 1.5f;
            }
        }

        glm::vec2 points[4] = {
                pos + _cursor_style.off + glm::vec2(xOff, 0),
                pos + _cursor_style.off + glm::vec2(xOff, 0) + glm::vec2(_cursor_style.dim.x,   0),
                pos + _cursor_style.off + glm::vec2(xOff, 0) + glm::vec2(_cursor_style.dim.x,   -_cursor_style.dim.y),
                pos + _cursor_style.off + glm::vec2(xOff, 0) + glm::vec2(0,                     -_cursor_style.dim.y),
        };

        // Dirty way of keeping the cursor always within the textbox
        points[0].x = std::max(points[0].x, pos.x);
        points[3].x = std::max(points[3].x, pos.x);
        points[1].x = std::max(points[1].x, points[0].x + _cursor_style.dim.x);
        points[2].x = std::max(points[2].x, points[3].x + _cursor_style.dim.x);

        points[1].x = std::min(points[1].x, pos.x + dim.x);
        points[2].x = std::min(points[2].x, pos.x + dim.x);
        points[0].x = std::min(points[0].x, points[1].x - _cursor_style.dim.x);
        points[3].x = std::min(points[3].x, points[2].x - _cursor_style.dim.x);

        renderer->renderQuad(
                points[0],
                points[1],
                points[2],
                points[3],
                _font_options.color,
                layer,
                &_opt
                );
    }

    float Textbox::get_text_len(const std::string& text) {
        float textWidth = 0;

        for (char c : text) {
            float cWidth = 0;
            if(c == ' ')       { cWidth = _whitespace;    } // Whitespace
            else if(c == '\t') { cWidth = _whitespace * 4;} // Tab
            else if(c == '\r') { cWidth = 0;          } // Carriage Return
            else if(c == '\n') { cWidth = 0;          } // Carriage Return
            else {
                Font::Glyph* g = _font_options.font->getGlyph(c);
                if(!g)
                    continue;
                cWidth = (g->dim.x * _font_scale) + (g->offset.x * _font_scale);
            }

            if(c > 20)
                textWidth += _whitespace * _font_options.spacing.x;
            textWidth += cWidth;
        }

        return textWidth;
    }

    void Textbox::update_font_scale() {
        Font* font = _font_options.font;
        Font::Glyph* g = font->getGlyph('A');
        _font_scale =  _font_size / g->dim.y;;
        _whitespace = g->dim.x * _font_scale;
    }

    void Textbox::reset_blink() {
        _blink_time = _blink_max_time * 1.5;
    }

    float Textbox::get_char_len(char c) const {
        float cWidth = 0;
        if(c == ' ')       { cWidth = _whitespace;    } // Whitespace
        else if(c == '\t') { cWidth = _whitespace * 4;} // Tab
        else if(c == '\r') { cWidth = 0;          } // Carriage Return
        else if(c == '\n') { cWidth = 0;          } // Carriage Return
        else {
            Font::Glyph* g = _font_options.font->getGlyph(c);
            if(g)
                cWidth = (g->dim.x * _font_scale) + (g->offset.x * _font_scale);
        }

        return cWidth;
    }


}