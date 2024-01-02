#ifndef KIKAN_IUIELEMENT_H
#define KIKAN_IUIELEMENT_H

#include <glm/glm.hpp>

namespace Kikan {
    class IUIElement {
    public:
        glm::vec2 pos = glm::vec2(0);
        glm::vec2 dim = glm::vec2(0);

        virtual void render() = 0;
    };
}

#endif //KIKAN_IUIELEMENT_H
