#include <utility>
#include <algorithm>

#include "Kikan/ui/elements/Textbox.h"
#include "Kikan/Engine.h"


namespace Kikan{

    void textboxOnClick(IInteractable* interactable, IInteractable::State state, void* data){
        auto* textbox = (Textbox*)interactable;
        auto* ui = Engine::Kikan()->getUI();
        glm::vec2 mouse = ui->getUIMousePos();

        textbox->setCursor(mouse.x - ui->getPos(textbox).x);
        textbox->setHold();
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

        registerCallback(textboxOnClick, IInteractable::PRESSED);
        _text_queue_id = Engine::Kikan()->getInput()->registerTextQueue();
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

        if(_cursor != _select_cursor)
            render_select();
    }

    void Textbox::update() {
        auto* input = Engine::Kikan()->getInput();
        if(_holding_mouse && !input->mousePressed(Mouse::BUTTON_LEFT))
            _holding_mouse = false;

        if(_holding_mouse){
            auto* ui = Engine::Kikan()->getUI();
            glm::vec2 mouse = ui->getUIMousePos();
            float  offset = mouse.x - ui->getPos(this).x;

            setCursor(offset);
            // TODO: make framerate independent
            if(offset < 0){
                setCursor(_cursor - 1);
            }
            else if(offset > dim.x){
                setCursor(_cursor + 1);
            }
        }

        if(!focused){
            input->clearTextQueue(_text_queue_id);
            return;
        }

        while(!input->isTextQueueEmpty(_text_queue_id)) {
            char c = input->popTextQueue(_text_queue_id);
            if(c > 0){
                if(_select_cursor > _cursor){
                    eraseText(_cursor, _select_cursor);
                    setCursor(_cursor);
                }
                else if(_select_cursor < _cursor){
                    eraseText(_select_cursor, _cursor);
                    setCursor(_select_cursor);
                }

                _text.insert(_cursor, 1, c);
                setCursor(_cursor + 1);
                reset_blink();
                _select_cursor = _cursor;
            }
        }

        if(input->keyPressed(Key::LEFT_CONTROL) || input->keyPressed(Key::RIGHT_CONTROL)){
            if(input->keyXPressed(Key::C) || input->keyHolding(Key::C)){
                std::string copy;
                if(_select_cursor > _cursor){
                    copy = _text.substr(_cursor, _select_cursor - _cursor);
                }
                else if(_select_cursor < _cursor){
                    copy = _text.substr(_select_cursor, _cursor - _select_cursor);
                }
                if(!copy.empty())
                    input->setClipboard((char *)(copy.c_str()));

                kikanPrintW("[WARNING] Copying is to clipboard discouraged as it can cause lag\n");
            }
            else if(input->keyXPressed(Key::X) || input->keyHolding(Key::X)){
                std::string copy;
                if(_select_cursor > _cursor){
                    copy = _text.substr(_cursor, _select_cursor - _cursor);
                    eraseText(_cursor, _select_cursor);
                    setCursor(_cursor);
                }
                else if(_select_cursor < _cursor){
                    copy = _text.substr(_select_cursor, _cursor - _select_cursor);
                    eraseText(_select_cursor, _cursor);
                    setCursor(_select_cursor);
                }
                if(!copy.empty())
                    input->setClipboard((char *)(copy.c_str()));

                kikanPrintW("[WARNING] Copying is to clipboard discouraged as it can cause lag\n");
            }
            else if(input->keyXPressed(Key::V) || input->keyHolding(Key::V)){
                std::string c(input->getClipboard());
                if(!c.empty()){
                    if(_select_cursor > _cursor){
                        eraseText(_cursor, _select_cursor);
                        setCursor(_cursor);
                    }
                    else if(_select_cursor < _cursor){
                        eraseText(_select_cursor, _cursor);
                        setCursor(_select_cursor);
                    }

                    _text.insert(_cursor, c);
                    setCursor(_cursor + (int32_t)c.size());
                    reset_blink();
                    _select_cursor = _cursor;
                }
            }
        }

        if(input->keyXPressed(Key::BACKSPACE) || input->keyHolding(Key::BACKSPACE)){
            int32_t begin = std::max(_cursor - 1, 0);
            int32_t end = _cursor;

            if(_cursor > _select_cursor){
                begin = _select_cursor;
                end = _cursor;
            }
            else if(_cursor < _select_cursor){
                begin = _cursor;
                end = _select_cursor;
                _select_cursor = begin;
            }
            else if(input->keyPressed(Key::LEFT_CONTROL) || input->keyPressed(Key::RIGHT_CONTROL)){
                begin = get_next_whitespace(_cursor, false);
            }

            _select_cursor = begin;
            eraseText(begin, end);
            setCursor(begin);
        }

        if(input->keyXPressed(Key::DELETE) || input->keyHolding(Key::DELETE)){
            int32_t begin = _cursor;
            int32_t end = std::min(_cursor + 1, (int32_t)_text.size());

            if(_cursor > _select_cursor){
                begin = _select_cursor;
                end = _cursor;
            }
            else if(_cursor < _select_cursor){
                begin = _cursor;
                end = _select_cursor;
            }
            else if(input->keyPressed(Key::LEFT_CONTROL) || input->keyPressed(Key::RIGHT_CONTROL)){
                end = get_next_whitespace(_cursor, true);
            }

            _select_cursor = begin;
            eraseText(begin, end);
            setCursor(begin);
        }

        if(input->keyXPressed(Key::END))
            setCursor((int32_t)_text.size());
        if(input->keyXPressed(Key::HOME))
            setCursor((int32_t)0);

        // TODO: Fix frame rate dependency
        if(input->keyXPressed(Key::LEFT) || input->keyHolding(Key::LEFT)){
            if(input->keyPressed(Key::LEFT_CONTROL) || input->keyPressed(Key::RIGHT_CONTROL))
                setCursor(get_next_whitespace(_cursor, false));
            else
                setCursor(_cursor - 1);
        }
        if(input->keyXPressed(Key::RIGHT) || input->keyHolding(Key::RIGHT)){
            if(input->keyPressed(Key::LEFT_CONTROL) || input->keyPressed(Key::RIGHT_CONTROL))
                setCursor(get_next_whitespace(_cursor, true));
            else
                setCursor(_cursor + 1);
        }

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

        auto* input = Engine::Kikan()->getInput();
        if(!input->keyPressed(Key::LEFT_SHIFT) && !input->keyPressed(Key::RIGHT_SHIFT) && !_holding_mouse)
            _select_cursor = _cursor;
    }

    void Textbox::setCursor(float offset) {
        std::string sub = _text.substr(_text_bound_l, _text_bound_r - _text_bound_l);
        float w = _text_offset.x;
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
            if(i == sub.size() - 1)
                setCursor((int32_t)(_text_bound_r));
        }
    }

    void Textbox::setText(std::string text) {
        if(_text_bound_r > text.size() - 1){
            _left_bound = false;
            _text_bound_r = text.size() - 1;
        }
        _text = std::move(text);

        setCursor((int32_t)_text.size());
    }

    std::string Textbox::getText() {
        return _text;
    }

    void Textbox::eraseText(int32_t begin, int32_t end) {
        begin = std::min(std::max(begin, 0), (int32_t)_text.size());
        end =   std::min(std::max(end,   0), (int32_t)_text.size());

       _text.erase(begin, end - begin);

        if(_text.size() < _text_bound_r){
            if(_cursor - 1 == (int32_t)_text.size()){
                _text_bound_r = _text.size();
                _left_bound = false;
            }
            else
                _left_bound = true;
        }

        if(_cursor > (int32_t)_text.size())
            setCursor((int32_t)_text.size());
    }

    void Textbox::destroy() {
        delete this;
    }

    void Textbox::calc_bounds() {
        if(_text.empty()) {
            _left_bound = true;
            _text_bound_l = 0;
            setCursor(0);
            return;
        }

        _cut_percentage = 1;
        float textWidth = 0;
        bool textTooLong = false;
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
                cWidth = (g->dim.x * _font_scale) + (g->offset.x * _font_scale);
            }

            float spacing = c < 21 ? 0 : _whitespace * _font_options.spacing.x;

            if(textWidth + cWidth >= (dim.x - 2 * _text_offset.x)){
                _cut_percentage = ((dim.x - 2 * _text_offset.x) - (textWidth + spacing))/cWidth;
                if(_left_bound)
                    _text_bound_r = i + 1;
                else
                    _text_bound_l = i;

                textTooLong = true;
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

        if(!_left_bound && !textTooLong){
            _left_bound = true;
            calc_bounds();
        }
    }

    void Textbox::render_text(const std::string& text, glm::vec2 pos) {
        if(_text.empty())
            return;

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
            if(c == ' ')       { x += _whitespace * yScale;     continue; }
            else if(c == '\t') { x += _whitespace * yScale * 4; continue; }
            else if(c == '\r') {                                continue; }
            else if(c == '\n') {                                continue; }

            Font::Glyph* g = _font_options.font->getGlyph(c);
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

        float xOff = get_cursor_off(_cursor);

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


    void Textbox::render_select() {
        float layer = Engine::Kikan()->getUI()->renderLayer + _layer_offset - 0.1f;
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();

        float rOff, lOff;
        if(_cursor < _select_cursor){
            rOff = get_cursor_off(_select_cursor);
            lOff = get_cursor_off(_cursor);
        }
        else{
            rOff = get_cursor_off(_cursor);
            lOff = get_cursor_off(_select_cursor);
        }

        glm::vec2 points[4] = {
                pos + _cursor_style.off + glm::vec2(lOff, 0),
                pos + _cursor_style.off + glm::vec2(rOff, 0) + glm::vec2(_cursor_style.dim.x, 0),
                pos + _cursor_style.off + glm::vec2(rOff, 0) + glm::vec2(_cursor_style.dim.x, -_cursor_style.dim.y),
                pos + _cursor_style.off + glm::vec2(lOff, 0) + glm::vec2(0, -_cursor_style.dim.y),
        };

        // Dirty way of keeping the select always within the textbox
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
                glm::vec4(.2,.4,.8,.2),
                layer,
                &_opt
        );
    }

    float Textbox::get_text_len(const std::string& text) const {
        float textWidth = 0;

        for (char c : text) {
            float cWidth = 0;
            if(c == ' ')       { cWidth = _whitespace;    } // Whitespace
            else if(c == '\t') { cWidth = _whitespace * 4;} // Tab
            else if(c == '\r') { cWidth = 0;          } // Carriage Return
            else if(c == '\n') { cWidth = 0;          } // Carriage Return
            else {
                Font::Glyph* g = _font_options.font->getGlyph(c);
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
            cWidth = (g->dim.x * _font_scale) + (g->offset.x * _font_scale);
        }

        return cWidth;
    }

    float Textbox::get_cursor_off(int32_t cursor) {
        float off = 0;
        if(cursor != 0){
            if(_left_bound){
                std::string sub = _text.substr(_text_bound_l, cursor - _text_bound_l);
                off = get_text_len(sub);
                if(cursor > 1 && _text[cursor - 1] == ' ')
                    off -= _whitespace * .2f;
                else
                    off -= _whitespace * _font_options.spacing.x * .5f;
            }
            else{
                std::string sub = _text.substr(cursor, _text_bound_r - cursor);
                off = (dim.x - 2 * _text_offset.x) - get_text_len(sub);
                if(cursor > 1 && _text[cursor - 1] == ' ')
                    off -= _whitespace * .2f;
                else
                    off -= _whitespace * _font_options.spacing.x * 1.5f;
            }
        }
        return off;
    }

    int32_t Textbox::getCursorPos() const {
        return _cursor;
    }

    void Textbox::setTextOffset(glm::vec2 off) {
        _text_offset = off;
    }

    glm::vec2 Textbox::getTextOffset() {
        return _text_offset;
    }

    void Textbox::setCursorStyle(Textbox::CursorStyle style) {
        _cursor_style = style;
    }

    Textbox::CursorStyle Textbox::getCursorStyle() {
        return _cursor_style;
    }

    void Textbox::setFont(Font::Options font) {
        _font_options = font;
    }

    struct Font::Options Textbox::getFont() {
        return _font_options;
    }

    int32_t Textbox::get_next_whitespace(int32_t start, bool rightDir) {
        int32_t inc = rightDir ? 1 : -1;
        bool startWord = false;
        if(!rightDir)
            start -= 1;

        if(start >= 0 && start < _text.size()){
            if(_text[start] != ' ')
                startWord = true;
        }

        for(int32_t i = start ;; i += inc){
            if(i < 0)
                return 0;
            if(i > (int32_t )_text.size() - 1)
                return (int32_t )_text.size();

            char c = _text[i];
            if(c != ' ')
                startWord = true;
            else if(c == ' ' && startWord)
                return rightDir ? i : i + 1;
        }

        return start;
    }

    void Textbox::setHold() {
        _holding_mouse = true;
    }
}