#include "Kikan/input/stdInput/StdInput.h"
#include "Kikan/core/Logging.h"

namespace Kikan {

    StdInput::StdInput(void* params) {
        auto initParams = (struct InitParams*)params;
        if(!params){
            kikanPrintE("[ERROR] Input InitParams are null\n");
            return;
        }

        _window = initParams->window;

        glfwSetWindowUserPointer(_window, this);

        // setup Mouse Button callback
        auto mouse_btn = [](GLFWwindow* w, int b, int a, int m){
            static_cast<StdInput*>(glfwGetWindowUserPointer(w))->mouse_btn_callback( b, a, m);
        };
        glfwSetMouseButtonCallback(_window, mouse_btn);

        // setup Mouse Position callback
        auto mouse_pos = [](GLFWwindow* w, double x, double y){
            static_cast<StdInput*>(glfwGetWindowUserPointer(w))->mouse_pos_callback(x, y);
        };
        glfwSetCursorPosCallback(_window, mouse_pos);

        // setup Key callback
        auto key = [](GLFWwindow* w, int k, int s, int a, int m){
            static_cast<StdInput*>(glfwGetWindowUserPointer(w))->key_callback( k, s, a, m);
        };
        glfwSetKeyCallback(_window, key);

        auto text = [](GLFWwindow* w, unsigned int c){
            static_cast<StdInput*>(glfwGetWindowUserPointer(w))->char_callback(c);
        };
        glfwSetCharCallback(_window, text);


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

    StdInput::~StdInput() {
        for(auto p : _text_queues)
            delete p.second;
    }

    void StdInput::mouse_btn_callback(int btn, int action, int mods) {
        _m_keys[btn] = (action == GLFW_PRESS);
    }

    void StdInput::mouse_pos_callback(double x, double y) {
        _mouse_x = x;
        _mouse_y = y;
    }

    void StdInput::key_callback(int key, int scancode, int action, int mods) {
        _immediate_keys[(Key)key] = glfw_to_keystate(action);
        _queue_keys[(Key)key] = action;
    }

    void StdInput::char_callback(unsigned int codepoint) {
        for(auto p : _text_queues)
            p.second->push(codepoint);
    }

    double StdInput::mouseX() const {
        return _mouse_x;
    }

    double StdInput::mouseY() const {
        return _mouse_y;
    }

    void StdInput::mouseP(glm::vec2 &pos) const {
        pos.x = (float)_mouse_x;
        pos.y = (float)_mouse_y;
    }

    bool StdInput::mousePressed(Mouse m) {
        return _m_keys[m];
    }

    void StdInput::update() {
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

    bool StdInput::keyPressed(Key k) {
        return (_keys[k] == KeyState::PRESSED) || (_keys[k] == KeyState::PRESSING || (_keys[k] == KeyState::HOLDING));
    }

    bool StdInput::keyXPressed(Key k) {
        return _keys[k] == KeyState::PRESSED;
    }

    bool StdInput::keyPressing(Key k) {
        return (_keys[k] == KeyState::PRESSED) || (_keys[k] == KeyState::PRESSING);
    }

    bool StdInput::keyXPressing(Key k) {
        return _keys[k] == KeyState::PRESSING;
    }

    bool StdInput::keyHolding(Key k) {
        return _keys[k] == KeyState::HOLDING;
    }

    bool StdInput::keyReleased(Key k) {
        return _keys[k] == KeyState::RELEASED;
    }

    bool StdInput::keyNone(Key k) {
        return _keys[k] == KeyState::NONE;
    }

    bool StdInput::isKey(Key k, KeyState state) {
        return _keys[k] == state;
    }

    KeyState StdInput::keyState(Key k) {
        return _keys[k];
    }

    KeyState StdInput::iKeyState(Key k) {
        return _immediate_keys[k];
    }

    KeyState StdInput::glfw_to_keystate(int action) {
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

    Key StdInput::lastKey() {
        return _last_key;
    }

    const char* StdInput::getClipboard() {
        return glfwGetClipboardString(_window);
    }

    /*
     * !!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * While string is in clipboard can cause lag
     *
    */
    void StdInput::setClipboard(char *content) {
        glfwSetClipboardString(_window, content);
    }

    uint32_t StdInput::registerTextQueue() {
        uint32_t id = _next_queue_id;
        _next_queue_id++;
        _text_queues[id] = new std::queue<uint32_t>;
        return id;
    }

    void StdInput::unregisterTextQueue(uint32_t id) {
        delete _text_queues[id];
        _text_queues[id] = nullptr;
    }

    uint32_t StdInput::getTextQueue(uint32_t id) {
        if(isTextQueueEmpty(id))
            return 0;

        return _text_queues[id]->front();
    }

    uint32_t StdInput::popTextQueue(uint32_t id) {
        if(isTextQueueEmpty(id))
            return 0;

        uint32_t c = _text_queues[id]->front();
        _text_queues[id]->pop();
        return c;
    }

    bool StdInput::isTextQueueEmpty(uint32_t id) {
        return _text_queues[id]->empty();
    }

    void StdInput::clearTextQueue(uint32_t id) {
        while(!isTextQueueEmpty(id))
            popTextQueue(id);
    }
}
