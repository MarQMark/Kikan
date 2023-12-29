#include "Kikan/renderer/stdRenderer/Font.h"
#include "Kikan/renderer/stdRenderer/DefaultFont.h"
#include "Kikan/core/Logging.h"

namespace Kikan{

    Font::Font(void *data, uint32_t size) {
        parse_font((int8_t*)data, size);
    }

    Font::Glyph* Font::getGlyph(char c) {
        if(!_glyphs.count(c)){
            kikanPrintE("[ERROR] %c not found in font\n", c);
            return _glyphs.at(0);
        }

        return _glyphs[c];
    }

    GLuint Font::getID() {
        return _txt->get();
    }

    void Font::parse_font(int8_t *data, uint32_t size) {
        uint32_t width, height;
        width = *(uint32_t*)data;
        data+= 4;
        height = *(uint32_t*)data;
        data+= 4;

        for(int i = 0; i < 82; i++){
            char c = *data;
            data++;
            auto* g = new Glyph;
            g->dim.x = (float)*(uint32_t*)data / (float)width;
            data+= 4;
            g->dim.y = (float)*(uint32_t*)data / (float)height;
            data+= 4;
            g->pos.x = (float)*(uint32_t*)data / (float)width;
            data+= 4;
            g->pos.y = (float)*(uint32_t*)data / (float)height;
            data+= 4;
            _glyphs[c] = g;
        }
    }
}