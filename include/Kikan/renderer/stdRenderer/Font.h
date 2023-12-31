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
        struct Options{
            Font* font;
            glm::vec4 color = glm::vec4(1);
            glm::vec2 spacing = glm::vec2(1.1f);
        };

    public:
        explicit Font(void* data);
        ~Font();

        Glyph* getGlyph(char c);
        GLuint getID();

    private:
        typedef struct __attribute__((packed)) {
            uint32_t width;     // texture2D width
            uint32_t height;    // texture2D height
            uint32_t gCount;    // glyph count
            uint32_t gWidth;    // glyph max width
            uint32_t gHeight;   // glyph max height
        } Header;

        std::map<char, Glyph*> _glyphs;
        Texture2D* _txt;

        void parse_font(int8_t* data);
    };
}

#endif //KIKAN_FONT_H
