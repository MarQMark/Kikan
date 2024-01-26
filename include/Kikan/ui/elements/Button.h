#ifndef KIKAN_BUTTON_H
#define KIKAN_BUTTON_H

#include "IInteractable.h"

namespace Kikan {
    class Button : public IInteractable {
    public:
        Button(std::string name, glm::vec2 pos, glm::vec2 dim, std::string text = "");

        void setColor(glm::vec4 color);
        void setHoverColor(glm::vec4 color);
        void setHoldColor(glm::vec4 color);
        void setDisabledColor(glm::vec4 color);
        glm::vec4 getColor();
        glm::vec4 getHoverColor();
        glm::vec4 getHoldColor();
        glm::vec4 getDisabledColor();

        void setLayerOffset(float offset);
        float getLayerOffset();

        void setText(std::string text);
        std::string getText();
        void setFontOptions(Font::Options options);
        Font::Options getFontOptions();
        void setFontSize(float size);
        float getFontSize() const;
        void setTextOffset(glm::vec2 offset);
        glm::vec2 getTextOffset();

        void render(glm::vec2 parentPos) override;

        void destroy() override;
    private:
        glm::vec4 _base_color = glm::vec4(.2f, .4f, .8f, 1.f);
        glm::vec4 _hover_color;
        glm::vec4 _hold_color;
        glm::vec4 _disabled_color;
        bool _custom_hover_color = false;
        bool _custom_hold_color = false;

        float _layer_offset = -0.01;

        float _focus_border;

        std::string _text;
        Font::Options _font_options;
        float _font_size;
        glm::vec2 _text_offset;
        float _font_layer_offset = -0.001f;

        void adjust_colors();
    };
}

#endif //KIKAN_BUTTON_H
