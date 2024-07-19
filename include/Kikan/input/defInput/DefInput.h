#ifndef KIKAN_DEFINPUT_H
#define KIKAN_DEFINPUT_H

#include "../IStdInput.h"

namespace Kikan {

    class DefInput : public IStdInput {
    public:
        explicit DefInput(IStdInput* input);

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
        IStdInput* _input = {};
    };
}

#endif //KIKAN_DEFINPUT_H
