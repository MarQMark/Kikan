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
        void setHeight(float height);

        /*
         * Using this is discouraged if not strictly necessary
         * For scaling the width and height please use setDimensions()
         *
         * If this is used the aspect ratio will no longer be maintained
         */
        void setMVP(glm::mat4x4 mvp);
        glm::mat4x4 getMVP();
        void resetMVP();

        void addElement(IUIElement* element);

        void update();
    private:
        std::vector<IInteractable*> _interactables;
        std::vector<IUIElement*> _ui_elements;

        float _width, _height;
        glm::mat4x4 _mvp{};
        glm::mat4x4 _i_mvp{};
        bool _custom_mvp = false;

        bool _prev_left_click = false;

        /*
         *  Does not preform the render call only adds vertices to batch
         */
        void render();
    };
}



#endif //KIKAN_UI_H
