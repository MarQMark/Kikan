#ifndef KIKAN_I_RENDER_SYSTEM_H
#define KIKAN_I_RENDER_SYSTEM_H

#include "ISystem.h"
#include "Kikan/renderer/stdRenderer/StdRenderer.h"

namespace Kikan {
    class IRenderSystem : public ISystem {
    public:
        IRenderSystem() = default;

        void setRenderer(Renderer::StdRenderer* renderer){
            _renderer = renderer;
        }
    protected:
        Renderer::StdRenderer* _renderer{};
    };
}

#endif //KIKAN_I_RENDER_SYSTEM_H
