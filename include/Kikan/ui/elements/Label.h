#ifndef KIKAN_LABEL_H
#define KIKAN_LABEL_H

#include <string>
#include "IUIElement.h"
#include "glm/glm.hpp"
#include "Kikan/renderer/stdRenderer/buffers/Texture2D.h"
#include "Kikan/renderer/stdRenderer/Font.h"
#include "Kikan/renderer/stdRenderer/StdRenderer.h"

namespace Kikan {
    class Label : public IUIElement {
    public:
        Label(std::string name, glm::vec2 pos, glm::vec2 dim, std::string text);
        Label(std::string name, glm::vec2 pos, glm::vec2 dim, Texture2D* txt);

        void setBgLayerOffset(float offset);
        float getBgLayerOffset() const;
        void setBgColor(glm::vec4 color);
        glm::vec4 getBgColor();

        void setText(std::string text);
        std::string getText();
        void setFontOptions(Font::Options options);
        Font::Options getFontOptions();
        void setFontSize(float size);
        float getFontSize() const;
        void setFontLayerOffset(float offset);
        float getFontLayerOffset() const;

        void setTexture2D(Texture2D* txt);
        Texture2D* getTexture();
        void setTexturePos(glm::vec2 txtPos[]);
        void getTexturePos(glm::vec2 txtPos[]);
        void setTextureCoords(glm::vec2 txtCoords[]);
        void getTextureCoords(glm::vec2 txtCoords[]);
        void setTextureLayerOffset(float offset);
        float getTextureLayerOffset() const;

        void render(glm::vec2 parentPos) override;

        void destroy() override;
    private:
        float _bg_layer_offset = -0.01f;
        glm::vec4 _bg_color = glm::vec4(0.5);

        // Font
        std::string _text;
        Font::Options _font_options;
        float _font_size;
        float _font_layer_offset = -0.012f;

        // Texture
        Texture2D* _txt{};
        glm::vec2 _txt_pos[4];
        glm::vec2 _txt_coords[4] = {
                glm::vec2(0, 1),
                glm::vec2(1, 1),
                glm::vec2(1, 0),
                glm::vec2(0, 0),
        };
        float _txt_layer_offset  = -0.011f;

        void init(glm::vec2 pos, glm::vec2 dim);
    };
}

#endif //KIKAN_LABEL_H
