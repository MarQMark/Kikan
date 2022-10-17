#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "Engine.h"

int WinMain() {
    Engine engine;
    GLFWwindow* window = engine.getRenderer()->getWindow();

    /* Loop until the user closes the _window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    return 0;
}

#ifdef __linux__
int main(){
    WinMain();
}
#endif
