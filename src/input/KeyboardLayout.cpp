#include "Kikan/input/KeyboardLayout.h"

namespace Kikan {

    char KeyboardLayout::getChar(Key key, KeyboardLayout::Modifier mod) {
        char c = 0;
        if(key >= Key::A && key <= Key::Z){
            c = 'a' + (key - Key::A);
            if(mod == Modifier::SHIFT)
                c -= 'a' - 'A';
        }
        else if(key == Key::SPACE)
            c = ' ';

        return c;
    }

    bool KeyboardLayout::isMod(Key key) {
        switch (key) {
            case Key::LEFT_SHIFT:
            case Key::RIGHT_SHIFT:
            case Key::LEFT_ALT:
            case Key::RIGHT_ALT:
            case Key::LEFT_CONTROL:
            case Key::RIGHT_CONTROL:
            case Key::LEFT_SUPER:
            case Key::RIGHT_SUPER:
                return true;
            default:
                return false;
        }
    }
}