#include "Renderer.h"
#include <iostream>

void Renderer::setup_openGl() {
    /* Initialize the library */
    if (!glfwInit())
        std::cout << "ERROR: Could not initialize GLFW" << std::endl;

    /* Create a windowed mode _window and its OpenGL context */
    _window = glfwCreateWindow(1280, 720, "Kikan", NULL, NULL);
    if (!_window) {
        glfwTerminate();
        std::cout << "ERROR: Could create Window" << std::endl;
    }

    /* Make the _window's context current */
    glfwMakeContextCurrent(_window);

    //disable VSYNC
    glfwSwapInterval(0);

    if(glewInit() != GLEW_OK)
        std::cout << "ERROR: Could not initialize GLEW" << std::endl;

    _shaders["default"] = new Shader("shaders/default.vert", "shaders/default.frag");
}

GLFWwindow *Renderer::getWindow() {
    return _window;
}

void Renderer::render(double dt) {
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto batch : _batches)
        batch.second->render();

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);

    /* Poll for and process events */
    glfwPollEvents();
}
