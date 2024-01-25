#include "Kikan/renderer/stdRenderer/RenderCallbacks.h"

namespace Kikan {
    void defaultPreRender(AutoBatch* batch, void* data){
        auto* shader = ((StdRenderer*)Engine::Kikan()->getRenderer())->shader();
        shader->bind();
        shader->uniform1li("u_sampler", 0);
        shader->uniformM4fv("u_mvp", ((StdRenderer*)Engine::Kikan()->getRenderer())->mvp);
    }

    void defaultTextPreRender(AutoBatch* batch, void* data){
        auto* shader = ((StdRenderer*)Engine::Kikan()->getRenderer())->shader("default_text");
        shader->bind();
        shader->uniform1li("u_sampler", 0);
        shader->uniformM4fv("u_mvp", ((StdRenderer*)Engine::Kikan()->getRenderer())->mvp);
    }
}