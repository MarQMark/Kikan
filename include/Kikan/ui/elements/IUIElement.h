#ifndef KIKAN_IUIELEMENT_H
#define KIKAN_IUIELEMENT_H

#include <glm/glm.hpp>
#include "Kikan/renderer/stdRenderer/StdRenderer.h"

namespace Kikan {
    class IUIElement {
    public:
        IUIElement(std::string name);
        virtual ~IUIElement() = default;

        glm::vec2 pos = glm::vec2(0);
        glm::vec2 dim = glm::vec2(0);

        bool enabled = true;

        struct StdRenderer::Options _opt;

        std::string getName(){
            return _name;
        }

        virtual void render(glm::vec2 parentPos) = 0;

        virtual void destroy() = 0;
    private:
        std::string _name;
    };
}

#endif //KIKAN_IUIELEMENT_H
