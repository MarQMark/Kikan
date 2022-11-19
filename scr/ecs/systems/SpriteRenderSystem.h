#ifndef KIKAN_SPRITE_RENDER_SYSTEM_H
#define KIKAN_SPRITE_RENDER_SYSTEM_H

#include "IRenderSystem.h"

class SpriteRenderSystem : public IRenderSystem{
public:
    SpriteRenderSystem();

    void update(double dt) override;
};


#endif //KIKAN_SPRITE_RENDER_SYSTEM_H
