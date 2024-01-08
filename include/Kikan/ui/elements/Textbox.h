#ifndef KIKAN_TEXTBOX_H
#define KIKAN_TEXTBOX_H

#include "IInteractable.h"

namespace Kikan {
    class Textbox : public IInteractable{
    public:
        struct CursorStyle{
            glm::vec2 dim;
            glm::vec2 off;
        };

        Textbox(std::string name, glm::vec2 pos, glm::vec2 dim);

        void setText(std::string text);
        std::string getText();

        void setCursor(int32_t cursor);
        void setCursor(float offset);

        void render(glm::vec2 parentPos) override;
        void update() override;

        void destroy() override;
    private:
        std::string _text;
        std::string _render_text;
        uint32_t _text_bound_l = 0;
        uint32_t _text_bound_r = 0;
        float _cut_percentage = 0;
        bool _left_bound = true;

        glm::vec2 _text_offset = glm::vec2(0);

        int32_t _cursor = 0;
        CursorStyle _cursor_style;
        const double _blink_max_time = 800.;
        double _blink_time = _blink_max_time;
        void reset_blink();

        bool _left = false;
        bool _right = false;

        float _layer_offset;

        float _font_size;
        float _font_scale;
        float _whitespace;
        Font::Options _font_options;

        void update_font_scale();
        float get_text_len(const std::string& text);
        float get_char_len(char c) const;

        void calc_bounds();

        void render_text(const std::string& text, glm::vec2 pos);
        void render_outline();
        void render_cursor();

    };
}


#endif //KIKAN_TEXTBOX_H
