#ifndef KIKAN_FONT_H
#define KIKAN_FONT_H

#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"
#include "glm/glm.hpp"
#include <map>

namespace Kikan{
    class Font {
    public:
        struct Glyph{
            glm::vec2 offset;
            glm::vec2 pos;
            glm::vec2 dim;
        };

    public:
        Font(void* data, uint32_t size);

        Glyph* getGlyph(char c);
        GLuint getID();

    private:
        std::map<char, Glyph*> _glyphs;
        Renderer::Texture2D* _txt;

        void parse_font(int8_t* data, uint32_t size);
    };
}

#endif //KIKAN_FONT_H
