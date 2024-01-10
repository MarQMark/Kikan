#ifndef KIKAN_KEYBOARDLAYOUT_H
#define KIKAN_KEYBOARDLAYOUT_H

#include "Input.h"

namespace Kikan {
    class KeyboardLayout {
    public:
        enum Modifier{
            NONE,
            SHIFT,
            CTRL,
            ALT,
            CTRL_ALT
        };

        static char getChar(Key key, Modifier mod = Modifier::NONE);
        static bool isMod(Key key);

    private:
    };
}


#endif //KIKAN_KEYBOARDLAYOUT_H
