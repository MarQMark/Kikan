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
        void eraseText(int32_t begin, int32_t end);

        void setTooltip(std::string tooltip);
        std::string getTooltip();
        void setTooltipColor(glm::vec4 color);
        glm::vec4 getToolTipColor();

        void setCursor(int32_t cursor);
        void setCursor(float offset);
        int32_t getCursorPos() const;

        void setTextOffset(glm::vec2 off);
        glm::vec2 getTextOffset();

        void setCursorStyle(struct CursorStyle style);
        struct CursorStyle getCursorStyle();

        void setFont(struct Font::Options font);
        struct Font::Options getFont();

        
        void setOutlineThickness(float thickness);
        float getOutlineThickness() const;
        void setOutlineColor(glm::vec4 outlineColor);
        glm::vec4 getOutlineColor() const;

        /*
         * This function is for internal use only
         */
        void setHold();

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

        std::string _tooltip;
        glm::vec4 _tooltip_color = glm::vec4(.3,.3,.3,1);

        glm::vec2 _text_offset = glm::vec2(0);

        int32_t _cursor = 0;
        int32_t _select_cursor = 0;
        CursorStyle _cursor_style;
        const double _blink_max_time = 800.;
        double _blink_time = _blink_max_time;
        void reset_blink();

        float _layer_offset = -.01f;

        float _font_size;
        float _font_scale;
        float _whitespace;
        Font::Options _font_options;

        float _outline_thickness = .2;
        glm::vec4 _outline_color = glm::vec4(1);

        uint32_t _text_queue_id;

        bool _holding_mouse = false;

        void update_font_scale();
        float get_text_len(const std::string& text) const;
        float get_char_len(char c) const;
        float get_cursor_off(int32_t cursor);
        int32_t get_next_whitespace(int32_t start,  bool rightDir);

        uint32_t get_tooltip_bound();

        void calc_bounds();

        void render_text(const std::string& text, glm::vec2 pos, glm::vec4 color);
        void render_outline();
        void render_cursor();
        void render_select();
    };
}


#endif //KIKAN_TEXTBOX_H
