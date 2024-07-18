project("KikanStdRenderer")

#add all source files
set(SOURCE_FILES
        ../../glew/glew.c

        StdRenderer.cpp
        Shader.cpp
        batching/Batch.cpp
        buffers/VertexBuffer.cpp
        batching/AutoBatch.cpp
        batching/ManuelBatch.cpp
        VertexRegistry.cpp
        buffers/IndexBuffer.cpp
        buffers/Texture2D.cpp
        Camera.cpp
        Font.cpp
        RenderCallbacks.cpp
        )

add_library(KikanStdRenderer SHARED ${SOURCE_FILES})

target_include_directories(KikanStdRenderer
        PUBLIC ../../../include
        PUBLIC ../../../external/glfw/include
)
target_link_directories(KikanStdRenderer PRIVATE external/glfw/src)

# Link Kikan Renderer
target_link_libraries(KikanStdRenderer glfw)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(KikanStdRenderer glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(KikanStdRenderer opengl32)
endif(WIN32)