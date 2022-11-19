#ifndef KIKAN_RENDERER_H
#define KIKAN_RENDERER_H

#include <map>
#include <string>
#include <cstring>
#include <glm/glm.hpp>
#include "Shader.h"
#include "batching/AutoBatch.h"
#include "vertices/DefaultVertex.h"
#include "vertices/VertexRegistry.h"

class Renderer {
public:
    Renderer(){
        setup_openGl();
    }

    ~Renderer(){
        glfwTerminate();
    }

    GLFWwindow* getWindow();


    void renderTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, int layer);
    void renderQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, glm::vec4 color, int layer);
    void renderPolygon(std::vector<glm::vec2>& points, glm::vec4 color, int layer);

    template <class T>
    void autoBatch(std::vector<IVertex*> vertices);
    template <class T>
    void autoBatch(std::vector<IVertex*> vertices, std::vector<GLuint>& indices);

    void render(double dt);

private:
    GLFWwindow *_window = nullptr;

    std::map<std::string, Shader*> _shaders;
    std::map<unsigned int, Batch*> _batches;

    std::map<unsigned int, std::vector<AutoBatch*>> _auto_batches;

    void setup_openGl();
};


#endif //KIKAN_RENDERER_H
