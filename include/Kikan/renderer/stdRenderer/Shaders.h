#ifndef KIKAN_SHADERS_H
#define KIKAN_SHADERS_H

// TODO: auto-gen this file

#include <string>

namespace Kikan { namespace Renderer {
        struct StdShaders{
        public:
            static const std::string sVS;
            static const std::string sFS;
        };

        const std::string StdShaders::sVS =
            "#version 430\n"
            "\n"
            "layout(location = 0) in float textureSlot;\n"
            "layout(location = 1) in vec4 position;\n"
            "layout(location = 2) in vec2 texCoords;\n"
            "layout(location = 3) in vec4 color;\n"
            "\n"
            "uniform mat4 u_mvp;\n"
            "\n"
            "out vec4 v_color;\n"
            "out vec2 v_texCoords;\n"
            "out float v_textureSlot;\n"
            "\n"
            "void main() {\n"
            "    v_color = color;\n"
            "    v_texCoords = texCoords;\n"
            "    v_textureSlot = textureSlot;\n"
            "    gl_Position = u_mvp * position;\n"
            "}";

        const std::string StdShaders::sFS =
                "#version 430 core\n"
                "\n"
                "in vec4 v_color;\n"
                "in vec2 v_texCoords;\n"
                "in float v_textureSlot;\n"
                "\n"
                "layout(location = 0) out vec4 fragColor;\n"
                "\n"
                "uniform sampler2D u_sampler;\n"
                "\n"
                "void main() {\n"
                "    vec4 color = v_color;\n"
                "\n"
                "    color = mix(color, texture(u_sampler, v_texCoords), step(0., v_textureSlot));\n"
                "\n"
                "    fragColor = color;\n"
                "}";

}   }

#endif //KIKAN_SHADERS_H
