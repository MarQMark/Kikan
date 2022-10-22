#ifndef KIKAN_RENDERER_H
#define KIKAN_RENDERER_H

#include "GL/glew.h"
#include <GLFW/glfw3.h>

class Renderer {
public:
    Renderer(){
        setup_openGl();
    }

    ~Renderer(){
        glfwTerminate();
    }

    GLFWwindow* getWindow();

    void render(double dt);

private:
    GLFWwindow *_window = nullptr;

    void setup_openGl();
};


#endif //KIKAN_RENDERER_H
