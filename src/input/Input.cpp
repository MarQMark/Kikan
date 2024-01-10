#include "Kikan/input/Input.h"
#include "Kikan/core/Logging.h"

namespace Kikan {
     Input *Input::create(GLFWwindow* window) {
        auto* input = new Input(window);

        glfwSetWindowUserPointer(window, input);

        // setup Mouse Button callback
        auto mouse_btn = [](GLFWwindow* w, int b, int a, int m){
            static_cast<Input*>(glfwGetWindowUserPointer(w))->mouse_btn_callback( b, a, m);
        };
        glfwSetMouseButtonCallback(window, mouse_btn);

        // setup Mouse Position callback
        auto mouse_pos = [](GLFWwindow* w, double x, double y){
            static_cast<Input*>(glfwGetWindowUserPointer(w))->mouse_pos_callback(x, y);
        };
        glfwSetCursorPosCallback(window, mouse_pos);

        // setup Key callback
        auto key = [](GLFWwindow* w, int k, int s, int a, int m){
            static_cast<Input*>(glfwGetWindowUserPointer(w))->key_callback( k, s, a, m);
        };
        glfwSetKeyCallback(window, key);

        return input;
    }

    Input::Input(GLFWwindow* window) {
        // init maps
        for (int i = -1; i < Key::LAST; ++i) {
            _keys[(Key)i] = KeyState::NONE;
            _queue_keys[(Key)i] = KeyState::NONE;
            _immediate_keys[(Key)i] = KeyState::NONE;
        }
        for (int i = 0; i < Mouse::BUTTON_LAST; ++i) {
            _m_keys[i] = false;
        }
    }

    void Input::mouse_btn_callback(int btn, int action, int mods) {
        _m_keys[btn] = (action == GLFW_PRESS);
    }

    void Input::mouse_pos_callback(double x, double y) {
        _mouse_x = x;
        _mouse_y = y;
    }

    void Input::key_callback(int key, int scancode, int action, int mods) {
         kikanPrint("key: %d\n", key);
        _immediate_keys[(Key)key] = glfw_to_keystate(action);
        _queue_keys[(Key)key] = action;
    }

    double Input::mouseX() const {
        return _mouse_x;
    }

    double Input::mouseY() const {
        return _mouse_y;
    }

    void Input::mouseP(glm::vec2 &pos) const {
        pos.x = (float)_mouse_x;
        pos.y = (float)_mouse_y;
    }

    bool Input::mousePressed(Mouse m) {
        return _m_keys[m];
    }

    void Input::update() {
        for(auto pair : _keys){
            Key k = pair.first;
            switch (pair.second) {
                case KeyState::NONE:
                    if(_queue_keys[k] == GLFW_PRESS || _queue_keys[k] == GLFW_REPEAT){
                        _last_key = k;
                        _keys[k] = KeyState::PRESSED;
                    }
                    break;
                case KeyState::PRESSED:
                    if(_queue_keys[k] == GLFW_PRESS)
                        _keys[k] = KeyState::PRESSING;
                    else if(_queue_keys[k] == GLFW_REPEAT) // This should normally not occur
                        _keys[k] = KeyState::HOLDING;
                    else if(_queue_keys[k] == GLFW_RELEASE)
                        _keys[k] = KeyState::RELEASED;
                    break;
                case KeyState::PRESSING:
                    if(_queue_keys[k] == GLFW_REPEAT) // This should normally not occur
                        _keys[k] = KeyState::HOLDING;
                    else if(_queue_keys[k] == GLFW_RELEASE)
                        _keys[k] = KeyState::RELEASED;
                    break;
                case KeyState::HOLDING:
                    if(_queue_keys[k] == GLFW_RELEASE)
                        _keys[k] = KeyState::RELEASED;
                    break;
                case KeyState::RELEASED:
                    _keys[k] = KeyState::NONE;
                    break;
            }

            if(_queue_keys[k] == GLFW_RELEASE)
                _queue_keys[k] = -1;
        }
    }

    bool Input::keyPressed(Key k) {
        return (_keys[k] == KeyState::PRESSED) || (_keys[k] == KeyState::PRESSING || (_keys[k] == KeyState::HOLDING));
    }

    bool Input::keyXPressed(Key k) {
        return _keys[k] == KeyState::PRESSED;
    }

    bool Input::keyPressing(Key k) {
        return (_keys[k] == KeyState::PRESSED) || (_keys[k] == KeyState::PRESSING);
    }

    bool Input::keyXPressing(Key k) {
        return _keys[k] == KeyState::PRESSING;
    }

    bool Input::keyHolding(Key k) {
        return _keys[k] == KeyState::HOLDING;
    }

    bool Input::keyReleased(Key k) {
        return _keys[k] == KeyState::RELEASED;
    }

    bool Input::keyNone(Key k) {
        return _keys[k] == KeyState::NONE;
    }

    bool Input::isKey(Key k, KeyState state) {
        return _keys[k] == state;
    }

    KeyState Input::keyState(Key k) {
        return _keys[k];
    }

    KeyState Input::iKeyState(Key k) {
        return _immediate_keys[k];
    }

    KeyState Input::glfw_to_keystate(int action) {
        switch (action) {
            case GLFW_RELEASE:
                return KeyState::RELEASED;
            case GLFW_PRESS:
                return KeyState::PRESSING;
            case GLFW_REPEAT:
                return KeyState::HOLDING;
            default:
                return KeyState::NONE;
        }
    }

    Key Input::lastKey() {
        return _last_key;
    }
}