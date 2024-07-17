#ifndef KIKAN_INPUT_H
#define KIKAN_INPUT_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <map>
#include <queue>

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

    class Input {
    public:
        struct InitParams{
            GLFWwindow* window;
        };

        static Input* create(void* params);
        ~Input();

        void update();

        Key lastKey();
        bool keyPressed(Key k);
        // Exclusive Pressed
        bool keyXPressed(Key k);
        bool keyPressing(Key k);
        // Exclusive Pressing
        bool keyXPressing(Key k);
        bool keyHolding(Key k);
        bool keyReleased(Key k);
        bool keyNone(Key k);
        bool isKey(Key k, KeyState state);
        KeyState keyState(Key k);
        /*
         *  Returns actual state of key
         *  This reduces input lag, but might also lead to key presses being missed, especially at low framerates
         */
        KeyState iKeyState(Key k);

        bool mousePressed(Mouse m);
        double mouseX() const;
        double mouseY() const;
        void mouseP(glm::vec2& pos) const;

        const char* getClipboard();
        void setClipboard(char* content);

        uint32_t registerTextQueue();
        void unregisterTextQueue(uint32_t id);
        uint32_t getTextQueue(uint32_t id);
        uint32_t popTextQueue(uint32_t id);
        bool isTextQueueEmpty(uint32_t id);
        void clearTextQueue(uint32_t id);

    private:
        explicit Input(GLFWwindow* window);

        void mouse_btn_callback(int btn, int action, int mods);
        void mouse_pos_callback(double x, double y);
        void key_callback(int key, int scancode, int action, int mods);
        void char_callback(unsigned int codepoint);

        std::map<Key, int> _queue_keys{};
        std::map<Key, KeyState> _immediate_keys{};
        std::map<Key, KeyState> _keys{};
        // When two keys get pressed withing the same frame the higher (ASCII) get chosen
        Key _last_key = Key::UNKNOWN;

        std::map<int, bool> _m_keys{};
        double _mouse_x = 0;
        double _mouse_y = 0;

        std::map<uint32_t, std::queue<uint32_t>*> _text_queues;
        uint32_t _next_queue_id = 0;

        GLFWwindow* _window{};

        KeyState glfw_to_keystate(int action);
    };
}

#endif //KIKAN_INPUT_H
