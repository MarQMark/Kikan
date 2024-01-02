#include "Kikan/ui/elements/IUIElement.h"
#include "Kikan/ui/elements/Helper.h"

namespace Kikan{
    IUIElement::IUIElement() {
        _opt.preRender = UIPreRender;
    }
}