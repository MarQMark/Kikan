project("KikanRenderer")

#add all source files
set(SOURCE_FILES
        ./../glew/glew.c

        stdRenderer/StdRenderer.cpp
        stdRenderer/Shader.cpp
        stdRenderer/batching/Batch.cpp
        stdRenderer/buffers/VertexBuffer.cpp
        stdRenderer/batching/AutoBatch.cpp
        stdRenderer/batching/ManuelBatch.cpp
        stdRenderer/VertexRegistry.cpp
        stdRenderer/buffers/IndexBuffer.cpp
        stdRenderer/buffers/Texture2D.cpp
        stdRenderer/Camera.cpp
        stdRenderer/Font.cpp
        )

add_library(KikanRenderer ${SOURCE_FILES})

target_include_directories(KikanRenderer
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../external/glfw/include
)
target_link_directories(KikanRenderer PRIVATE external/glfw/src)

# Link Kikan Renderer
target_link_libraries(KikanRenderer glfw)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(KikanRenderer glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(KikanRenderer opengl32)
endif(WIN32)