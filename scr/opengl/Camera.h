#ifndef KIKAN_CAMERA_H
#define KIKAN_CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Kikan {
    class Camera {
    public:
        Camera() {
            calc_matrix();
        }

        glm::mat4x4 matrix();

        void scale(float w, float h);

    private:
        float _width = 1;
        float _height = 1;

        void calc_matrix();

        glm::mat4x4 _matrix{};
    };
}

#endif //KIKAN_CAMERA_H
