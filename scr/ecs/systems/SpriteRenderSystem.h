#ifndef KIKAN_SPRITE_RENDER_SYSTEM_H
#define KIKAN_SPRITE_RENDER_SYSTEM_H

#include "ISystem.h"

class SpriteRenderSystem : public ISystem{
public:
    SpriteRenderSystem();

    void update(double dt) override;
};


#endif //KIKAN_SPRITE_RENDER_SYSTEM_H
