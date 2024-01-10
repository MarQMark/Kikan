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
        static char get_num_mod(Key k);
        static char get_var_mod(Key k);
        static char get_var(Key k);
    };
}


#endif //KIKAN_KEYBOARDLAYOUT_H
