#include "Kikan/ui/elements/IUIElement.h"
#include "Kikan/ui/elements/Helper.h"

namespace Kikan{
    IUIElement::IUIElement(std::string name) : _name(std::move(name)) {
        _opt.preRender = UIPreRender;
    }
}