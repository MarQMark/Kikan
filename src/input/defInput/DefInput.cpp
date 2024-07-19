#include "Kikan/input/defInput/DefInput.h"

namespace Kikan {
    Kikan::DefInput::DefInput(Kikan::IStdInput *input) {
        _input = input;
    }

    void Kikan::DefInput::update() {
        if(_input)
            _input->update();
    }

    Kikan::Key Kikan::DefInput::lastKey() {
        if(_input)
            return _input->lastKey();

        return Kikan::CAPS_LOCK;
    }

    bool Kikan::DefInput::keyPressed(Kikan::Key k) {
        if(_input)
            return _input->keyPressed(k);

        return false;
    }

    bool Kikan::DefInput::keyXPressed(Kikan::Key k) {
        if(_input)
            return _input->keyXPressed(k);

        return false;
    }

    bool Kikan::DefInput::keyPressing(Kikan::Key k) {
        if(_input)
            return _input->keyPressing(k);

        return false;
    }

    bool Kikan::DefInput::keyXPressing(Kikan::Key k) {
        if(_input)
            return _input->keyXPressing(k);

        return false;
    }

    bool Kikan::DefInput::keyHolding(Kikan::Key k) {
        if(_input)
            return _input->keyHolding(k);

        return false;
    }

    bool Kikan::DefInput::keyReleased(Kikan::Key k) {
        if(_input)
            return _input->keyReleased(k);

        return false;
    }

    bool Kikan::DefInput::keyNone(Kikan::Key k) {
        if(_input)
            return _input->keyNone(k);

        return false;
    }

    bool Kikan::DefInput::isKey(Kikan::Key k, Kikan::KeyState state) {
        if(_input)
            return _input->isKey(k, state);

        return false;
    }

    Kikan::KeyState Kikan::DefInput::keyState(Kikan::Key k) {
        if(_input)
            return _input->keyState(k);

        return Kikan::NONE;
    }

    Kikan::KeyState Kikan::DefInput::iKeyState(Kikan::Key k) {
        if(_input)
            return _input->iKeyState(k);

        return Kikan::NONE;
    }

    bool Kikan::DefInput::mousePressed(Kikan::Mouse m) {
        if(_input)
            return _input->mousePressed(m);

        return false;
    }

    double Kikan::DefInput::mouseX() const {
        if(_input)
            return _input->mouseX();

        return 0;
    }

    double Kikan::DefInput::mouseY() const {
        if(_input)
            return _input->mouseY();

        return 0;
    }

    void Kikan::DefInput::mouseP(glm::vec2 &pos) const {
        if(_input)
            _input->mouseP(pos);
    }

    const char *Kikan::DefInput::getClipboard() {
        if(_input)
            return _input->getClipboard();

        return nullptr;
    }

    void Kikan::DefInput::setClipboard(char *content) {
        if(_input)
            _input->setClipboard(content);
    }

    uint32_t Kikan::DefInput::registerTextQueue() {
        if(_input)
            return _input->registerTextQueue();

        return 0;
    }

    void Kikan::DefInput::unregisterTextQueue(uint32_t id) {
        if(_input)
            _input->unregisterTextQueue(id);
    }

    uint32_t Kikan::DefInput::getTextQueue(uint32_t id) {
        if(_input)
            return _input->getTextQueue(id);

        return 0;
    }

    uint32_t Kikan::DefInput::popTextQueue(uint32_t id) {
        if(_input)
            return _input->popTextQueue(id);

        return 0;
    }

    bool Kikan::DefInput::isTextQueueEmpty(uint32_t id) {
        if(_input)
            return _input->isTextQueueEmpty(id);

        return false;
    }

    void Kikan::DefInput::clearTextQueue(uint32_t id) {
        if(_input)
            _input->clearTextQueue(id);
    }
}