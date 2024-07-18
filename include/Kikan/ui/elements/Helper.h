#ifndef KIKAN_HELPER_H
#define KIKAN_HELPER_H

#include "Kikan/Engine.h"

namespace Kikan{

    void UIPreRender(AutoBatch* batch, void* data){
#if KIKAN_UI
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        auto* shader = renderer->shader();
        shader->bind();
        shader->uniformM4fv("u_mvp", Engine::Kikan()->getUI()->getMVP());
#endif
    }

    void UITextPreRender(AutoBatch* batch, void* data){
#if KIKAN_UI
        auto* renderer = (StdRenderer*)Engine::Kikan()->getRenderer();
        auto* shader = renderer->shader("default_text");
        shader->bind();
        shader->uniformM4fv("u_mvp", Engine::Kikan()->getUI()->getMVP());
#endif
    }

}

#endif //KIKAN_HELPER_H
