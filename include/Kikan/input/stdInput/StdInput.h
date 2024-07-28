#ifndef KIKAN_STDINPUT_H
#define KIKAN_STDINPUT_H

#include "../IStdInput.h"
#include <map>
#include <queue>

namespace Kikan {
    class StdInput : public IStdInput{
    public:
        struct InitParams{
            GLFWwindow* window{};
        };

        explicit StdInput(void* params);
        ~StdInput();

        void update() override;

        Key lastKey() override;
        bool keyPressed(Key k) override;
        // Exclusive Pressed
        bool keyXPressed(Key k) override;
        bool keyPressing(Key k) override;
        // Exclusive Pressing
        bool keyXPressing(Key k) override;
        bool keyHolding(Key k) override;
        bool keyReleased(Key k) override;
        bool keyNone(Key k) override;
        bool isKey(Key k, KeyState state) override;
        KeyState keyState(Key k) override;
        /*
         *  Returns actual state of key
         *  This reduces input lag, but might also lead to key presses being missed, especially at low framerates
         */
        KeyState iKeyState(Key k) override;

        bool mousePressed(Mouse m) override;
        double mouseX() const override;
        double mouseY() const override;
        void mouseP(glm::vec2& pos) const override;

        const char* getClipboard() override;
        void setClipboard(char* content) override;

        uint32_t registerTextQueue() override;
        void unregisterTextQueue(uint32_t id) override;
        uint32_t getTextQueue(uint32_t id) override;
        uint32_t popTextQueue(uint32_t id) override;
        bool isTextQueueEmpty(uint32_t id) override;
        void clearTextQueue(uint32_t id) override;

    private:

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

#endif //KIKAN_STDINPUT_H
