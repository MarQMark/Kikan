#include "Kikan/opengl/Camera.h"

namespace Kikan {
    glm::mat4x4 Camera::matrix() {
        calc_matrix();
        return _matrix;
    }

    void Camera::scale(float w, float h) {
        _width = w;
        _height = h;
    }

    void Camera::calc_matrix() {
        _matrix = glm::mat4x4(1.0f);
        _matrix = glm::scale(_matrix, glm::vec3(_width, _height, 1.0f));
    }
}