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
        cleanup_openGL();
    }

    GLFWwindow* getWindow();

private:
    GLFWwindow *_window = nullptr;

    void setup_openGl();
    void cleanup_openGL();
};


#endif //KIKAN_RENDERER_H
