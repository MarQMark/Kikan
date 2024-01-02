#ifndef KIKAN_HELPER_H
#define KIKAN_HELPER_H

#include "Kikan/Engine.h"

namespace Kikan{
    void UIPreRender(AutoBatch* batch, void* data){
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        renderer->shader()->uniformM4fv("u_mvp", Engine::Kikan()->getUI()->getMVP());
    }
}

#endif //KIKAN_HELPER_H
