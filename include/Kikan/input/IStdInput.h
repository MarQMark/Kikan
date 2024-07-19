#ifndef KIKAN_ISTDINPUT_H
#define KIKAN_ISTDINPUT_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace Kikan {

    enum Key{
        UNKNOWN = -1,
        SPACE = 32,
        APOSTROPHE = 39,
        COMMA = 44,
        MINUS = 45,
        PERIOD = 46,
        SLASH = 47,
        NUM_0 = 48,
        NUM_1 = 49,
        NUM_2 = 50,
        NUM_3 = 51,
        NUM_4 = 52,
        NUM_5 = 53,
        NUM_6 = 54,
        NUM_7 = 55,
        NUM_8 = 56,
        NUM_9 = 57,
        SEMICOLON = 59,
        EQUAL = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LEFT_BRACKET = 91,
        BACKSLASH = 92,
        RIGHT_BRACKET = 93,
        GRAVE_ACCENT = 96,
        WORLD_1 = 161,
        WORLD_2 = 162,
        ESCAPE = 256,
        ENTER = 257,
        TAB = 258,
        BACKSPACE = 259,
        INSERT = 260,
        DELETE = 261,
        RIGHT = 262,
        LEFT = 263,
        DOWN = 264,
        UP = 265,
        PAGE_UP = 266,
        PAGE_DOWN = 267,
        HOME = 268,
        END = 269,
        CAPS_LOCK = 280,
        SCROLL_LOCK = 281,
        NUM_LOCK = 282,
        PRINT_SCREEN = 283,
        PAUSE = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP_0 = 320,
        KP_1 = 321,
        KP_2 = 322,
        KP_3 = 323,
        KP_4 = 324,
        KP_5 = 325,
        KP_6 = 326,
        KP_7 = 327,
        KP_8 = 328,
        KP_9 = 329,
        KP_DECIMAL = 330,
        KP_DIVIDE = 331,
        KP_MULTIPLY = 332,
        KP_SUBTRACT = 333,
        KP_ADD = 334,
        KP_ENTER = 335,
        KP_EQUAL = 336,
        LEFT_SHIFT = 340,
        LEFT_CONTROL = 341,
        LEFT_ALT = 342,
        LEFT_SUPER = 343,
        RIGHT_SHIFT = 344,
        RIGHT_CONTROL = 345,
        RIGHT_ALT = 346,
        RIGHT_SUPER = 347,
        MENU = 348,
        LAST = GLFW_KEY_MENU
    };
    enum Mouse{
        BUTTON_1,
        BUTTON_2,
        BUTTON_3,
        BUTTON_4,
        BUTTON_5,
        BUTTON_6,
        BUTTON_7,
        BUTTON_8,
        BUTTON_LAST = BUTTON_8,
        BUTTON_LEFT = BUTTON_1,
        BUTTON_RIGHT = BUTTON_2,
        BUTTON_MIDDLE = BUTTON_3
    };

    /*
     * States explained:
     *      Pressed:    Only once for every key press. This is the first state
     *      Pressing:   While key is being held down but not marked as held
     *                  (For example: When you hold "a" it prints an a and after a short while it continuously prints "a"s.
     *                  This is the short time in between)
     *      Holding:    This state comes after Pressing
     *      Released:   Only once when key press is lifted
     *      None:       When none of the states above
     */
    enum KeyState{
        PRESSED,
        PRESSING,
        HOLDING,
        RELEASED,
        NONE,
    };

    class IStdInput {
    public:
        virtual ~IStdInput() = default;

        virtual void update() = 0;

        virtual Key lastKey() = 0;
        virtual bool keyPressed(Key k) = 0;
        // Exclusive Pressed
        virtual bool keyXPressed(Key k) = 0;
        virtual bool keyPressing(Key k) = 0;
        // Exclusive Pressing
        virtual bool keyXPressing(Key k) = 0;
        virtual bool keyHolding(Key k) = 0;
        virtual bool keyReleased(Key k) = 0;
        virtual bool keyNone(Key k) = 0;
        virtual bool isKey(Key k, KeyState state) = 0;
        virtual KeyState keyState(Key k) = 0;
        /*
         *  Returns actual state of key
         *  This reduces input lag, but might also lead to key presses being missed, especially at low framerates
         */
        virtual KeyState iKeyState(Key k) = 0;

        virtual bool mousePressed(Mouse m) = 0;
        virtual double mouseX() const = 0;
        virtual double mouseY() const = 0;
        virtual void mouseP(glm::vec2& pos) const = 0;

        virtual const char* getClipboard() = 0;
        virtual void setClipboard(char* content) = 0;

        virtual uint32_t registerTextQueue() = 0;
        virtual void unregisterTextQueue(uint32_t id) = 0;
        virtual uint32_t getTextQueue(uint32_t id) = 0;
        virtual uint32_t popTextQueue(uint32_t id) = 0;
        virtual bool isTextQueueEmpty(uint32_t id) = 0;
        virtual void clearTextQueue(uint32_t id) = 0;
    };
}

#endif //KIKAN_ISTDINPUT_H
