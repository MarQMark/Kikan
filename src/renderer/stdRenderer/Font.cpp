#include "Kikan/renderer/stdRenderer/Font.h"
#include "Kikan/renderer/stdRenderer/DefaultFont.h"
#include "Kikan/core/Logging.h"

namespace Kikan{

    Font::Font(void *data) {
        parse_font((int8_t*)data);
    }

    Font::~Font() {
        for (const auto &g: _glyphs) {
            delete g.second;
        }
    }

    Font::Glyph* Font::getGlyph(char c) {
        if(!_glyphs.count(c)){
            kikanPrintE("[ERROR] %c(%d) not found in font\n", c, c);
            return nullptr;
        }

        return _glyphs[c];
    }

    GLuint Font::getID() {
        return _txt->get();
    }

    void Font::parse_font(int8_t *data) {
        Header header = *(Header*)data;
        data += sizeof(Header);

        for(uint32_t i = 0; i < header.gCount; i++){
            char c = *data;
            data++;
            auto* g = new Glyph;
            g->dim.x = (float)*(uint32_t*)data / (float)header.width;       data+= 4;
            g->dim.y = (float)*(uint32_t*)data / (float)header.height;      data+= 4;
            g->pos.x = (float)*(uint32_t*)data / (float)header.width;       data+= 4;
            g->pos.y = (float)*(uint32_t*)data / (float)header.height;      data+= 4;
            g->offset.x = (float)*(uint32_t*)data / (float)header.width;    data+= 4;
            g->offset.y = (float)*(uint32_t*)data / (float)header.height;   data+= 4;
            _glyphs[c] = g;
        }

        uint8_t buff[header.width * header.height * 4];
        for(uint32_t i = 0; i < header.width * header.height; i++){
            buff[i * 4 + 0] = 255;      // Red
            buff[i * 4 + 1] = 255;      // Green
            buff[i * 4 + 2] = 255;      // Blue
            buff[i * 4 + 3] = *data;    // Alpha
            data++;
        }

        _txt = new Texture2D((GLsizei)header.width, (GLsizei)header.height, buff);
    }
}