#ifndef KIKAN_RENDERER_H
#define KIKAN_RENDERER_H

#include <map>
#include <string>
#include "Shader.h"
#include "Batch.h"

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

    std::map<std::string, Shader*> _shaders;
    std::map<unsigned int, Batch*> _batches;

    void setup_openGl();
};


#endif //KIKAN_RENDERER_H
