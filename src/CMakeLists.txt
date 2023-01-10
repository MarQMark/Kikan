project("Kikan")

#add all source files
set(SOURCE_FILES
        glew/glew.c
        Engine.cpp
        ecs/Scene.cpp
        ecs/Entity.cpp
        opengl/Renderer.cpp
        ecs/systems/PhysicsSystem.cpp
        ecs/TypeRegistry.cpp
        opengl/Shader.cpp
        opengl/batching/Batch.cpp
        opengl/buffers/VertexBuffer.cpp
        opengl/batching/AutoBatch.cpp
        opengl/batching/ManuelBatch.cpp
        opengl/VertexRegistry.cpp
        opengl/buffers/IndexBuffer.cpp
        opengl/buffers/Texture2D.cpp
        ecs/systems/SpriteRenderSystem.cpp
        opengl/Camera.cpp input/Input.cpp ../include/Kikan/input/Input.h)

add_library(Kikan ${SOURCE_FILES})

target_include_directories(Kikan
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)


#copy shaders to target folder
add_custom_target(copy_shaders ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Copying shaders into binary directory")
add_dependencies(Kikan copy_shaders)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link GLFW
find_library(LIB_GLFW
        NAMES libglfw3
        PATHS ${PROJECT_SOURCE_DIR}/../lib)
target_link_libraries(Kikan ${LIB_GLFW})

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Kikan glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(Kikan opengl32)
endif(WIN32)