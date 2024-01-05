#ifndef KIKAN_TEXTBOX_H
#define KIKAN_TEXTBOX_H

#include "IInteractable.h"

namespace Kikan {
    class Textbox : public IInteractable{
    public:
        Textbox(std::string name, glm::vec2 pos, glm::vec2 dim);

        void setText(std::string text);
        std::string getText();

        void render(glm::vec2 parentPos) override;
    private:
        std::string _text;
    };
}


#endif //KIKAN_TEXTBOX_H
