#include "Kikan/input/Input.h"

namespace Kikan {
    Input::Input(GLFWwindow* window) {

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

    }

    void Input::mouse_btn_callback(int btn, int action, int mods) {
    }

    void Input::mouse_pos_callback(double x, double y) {
    }

    void Input::key_callback(int key, int scancode, int action, int mods) {
    }
}