#ifndef KIKAN_RENDERCALLBACKS_H
#define KIKAN_RENDERCALLBACKS_H

#include "Kikan/Engine.h"

namespace Kikan {
    void defaultPreRender(AutoBatch* batch, void* data);
    void defaultTextPreRender(AutoBatch* batch, void* data);
}

#endif //KIKAN_RENDERCALLBACKS_H
