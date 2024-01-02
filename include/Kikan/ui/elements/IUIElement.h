#ifndef KIKAN_IUIELEMENT_H
#define KIKAN_IUIELEMENT_H

#include <glm/glm.hpp>
#include "Kikan/renderer/stdRenderer/StdRenderer.h"

namespace Kikan {
    class IUIElement {
    public:
        IUIElement();

        glm::vec2 pos = glm::vec2(0);
        glm::vec2 dim = glm::vec2(0);

        struct StdRenderer::Options _opt;

        virtual void render() = 0;
    };
}

#endif //KIKAN_IUIELEMENT_H
