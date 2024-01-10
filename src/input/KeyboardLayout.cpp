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
        else if(key >= Key::NUM_0 && key <= Key::NUM_9){
            if(mod == Modifier::SHIFT)
                c = get_num_mod(key);
            else
                c = '9' - (Key::NUM_9 - key);
        }
        else if(key >= Key::KP_0 && key <= Key::KP_9){
            c = '9' - (Key::KP_9 - key);
        }
        else{
            if(mod == Modifier::SHIFT)
                c = get_var_mod(key);
            else
                c = get_var(key);
        }

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

    char KeyboardLayout::get_num_mod(Key k) {
        switch (k) {
            case Key::NUM_1:
                return '!';
            case Key::NUM_2:
                return '@';
            case Key::NUM_3:
                return '#';
            case Key::NUM_4:
                return '$';
            case Key::NUM_5:
                return '%';
            case Key::NUM_6:
                return '^';
            case Key::NUM_7:
                return '&';
            case Key::NUM_8:
                return '*';
            case Key::NUM_9:
                return '(';
            case Key::NUM_0:
                return ')';
            default:
                return 0;
        }
    }


    char KeyboardLayout::get_var(Key k) {
        switch (k) {
            case GRAVE_ACCENT:
                return '`';
            case MINUS:
                return '-';
            case EQUAL:
                return '=';
            case LEFT_BRACKET:
                return '[';
            case RIGHT_BRACKET:
                return ']';
            case BACKSLASH:
                return '\\';
            case SEMICOLON:
                return ';';
            case APOSTROPHE:
                return '\'';
            case COMMA:
                return ',';
            case PERIOD:
                return '.';
            case SLASH:
            case KP_DIVIDE:
                return '/';
            case KP_MULTIPLY:
                return '*';
            case KP_SUBTRACT:
                return '-';
            case KP_ADD:
                return '+';
            case KP_DECIMAL:
                return '.';
            default:
                return 0;
        }
    }

    char KeyboardLayout::get_var_mod(Key k) {
        switch (k) {
            case GRAVE_ACCENT:
                return '~';
            case MINUS:
                return '_';
            case EQUAL:
                return '+';
            case LEFT_BRACKET:
                return '{';
            case RIGHT_BRACKET:
                return '}';
            case BACKSLASH:
                return '|';
            case SEMICOLON:
                return ':';
            case APOSTROPHE:
                return '\"';
            case COMMA:
                return '<';
            case PERIOD:
                return '>';
            case SLASH:
                return '?';
            default:
                return 0;
        }
    }
}