#ifndef KIKAN_I_RENDER_SYSTEM_H
#define KIKAN_I_RENDER_SYSTEM_H

#include "ISystem.h"
#include "Kikan/Engine.h"
#include "Kikan/renderer/stdRenderer/StdRenderer.h"

namespace Kikan {
    class IRenderSystem : public ISystem {
    public:
        IRenderSystem() {
            _renderer = (Renderer::StdRenderer*)Engine::Kikan()->getRenderer();
        };

    protected:
        Renderer::StdRenderer* _renderer{};
    };
}

#endif //KIKAN_I_RENDER_SYSTEM_H
