#ifndef KIKAN_UI_H
#define KIKAN_UI_H

#include <vector>
#include "glm/glm.hpp"

#include "Kikan/ui/elements/Interactable.h"

namespace Kikan {
    class UI {
    public:
        UI();
        ~UI();

        void setDimensions(uint32_t width, uint32_t height);

        /*
         * Using this is discouraged if not strictly necessary
         * For scaling the width and height please use setDimensions()
         *
         * If this is used it has to be set again after every resize event (subject to change)
         */
        void setMVP(glm::mat4x4 mvp);

        void addElement(UIElement* element);

        void render();
    private:
        std::vector<Interactable*> _interactables;
        std::vector<UIElement*> _ui_elements;

        uint32_t _width, _height;
        glm::mat4x4 _mvp{};
    };
}



#endif //KIKAN_UI_H
