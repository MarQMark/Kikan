#ifndef KIKAN_RENDERER_H
#define KIKAN_RENDERER_H

namespace Kikan { namespace Renderer {
    class Renderer {
    public:
        Renderer() = default;
        ~Renderer() = default;
        virtual void render(double dt) = 0;
        virtual void destroy() = 0;
    };
} }

#endif //KIKAN_RENDERER_H
