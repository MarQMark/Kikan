#include <utility>

#include "Kikan/ui/elements/Textbox.h"

namespace Kikan{

    Textbox::Textbox(std::string name, glm::vec2 pos, glm::vec2 dim) : IInteractable(std::move(name)) {
        this->pos = pos;
        this->dim = dim;
    }

    void Textbox::render(glm::vec2 parentPos) {

    }

    void Textbox::setText(std::string text) {
        _text = std::move(text);
    }

    std::string Textbox::getText() {
        return _text;
    }
}