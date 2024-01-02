#ifndef KIKAN_UI_H
#define KIKAN_UI_H

#include <vector>
#include "glm/glm.hpp"

#include "Kikan/ui/elements/IInteractable.h"

namespace Kikan {
    class UI {
    public:
        UI();
        ~UI();

        float renderLayer = -0.5;

        void setDimensions(float width, float height);

        /*
         * Using this is discouraged if not strictly necessary
         * For scaling the width and height please use setDimensions()
         *
         * If this is used it has to be set again after every resize event (subject to change)
         */
        void setMVP(glm::mat4x4 mvp);
        glm::mat4x4 getMVP();
        void resetMVP();

        void addElement(IUIElement* element);

        void update();
    private:
        std::vector<IInteractable*> _interactables;
        std::vector<IUIElement*> _ui_elements;

        uint32_t _width, _height;
        glm::mat4x4 _mvp{};

        /*
         *  Does not preform the render call only adds vertices to batch
         */
        void render();
    };
}



#endif //KIKAN_UI_H
