cmake_minimum_required(VERSION 3.23)
project(Kikan)

file(COPY shaders DESTINATION ./)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
endif(UNIX)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lmingw32")
endif(WIN32)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

#include external libraries
include_directories(${PROJECT_SOURCE_DIR}/include)

#add all source and header files
set(SOURCE_FILES scr/main.cpp scr/glew/glew.c scr/Engine.cpp scr/Engine.h scr/ecs/Scene.cpp scr/ecs/Scene.h scr/ecs/Entity.cpp scr/ecs/Entity.h scr/opengl/Renderer.cpp scr/opengl/Renderer.h scr/ecs/components/Physics.h scr/ecs/components/Transform.h scr/ecs/systems/ISystem.h scr/ecs/components/IComponent.h scr/ecs/systems/PhysicsSystem.cpp scr/ecs/systems/PhysicsSystem.h scr/ecs/TypeRegistry.h scr/ecs/TypeRegistry.cpp scr/opengl/Shader.cpp scr/opengl/Shader.h scr/opengl/batching/Batch.cpp scr/opengl/batching/Batch.h scr/opengl/buffers/VertexBuffer.cpp scr/opengl/buffers/VertexBuffer.h scr/opengl/vertices/IVertex.h scr/opengl/batching/AutoBatch.cpp scr/opengl/batching/AutoBatch.h scr/opengl/batching/ManuelBatch.cpp scr/opengl/batching/ManuelBatch.h scr/opengl/vertices/DefaultVertex.h scr/opengl/buffers/VertexBufferLayout.h scr/opengl/vertices/VertexRegistry.cpp scr/opengl/vertices/VertexRegistry.h scr/opengl/buffers/IndexBuffer.cpp scr/opengl/buffers/IndexBuffer.h scr/ecs/systems/SpriteRenderSystem.cpp scr/ecs/systems/SpriteRenderSystem.h scr/ecs/components/TriangleSprite.h scr/util/EarClipping.h scr/ecs/components/PolygonSprite.h scr/ecs/components/QuadSprite.h scr/ecs/systems/IRenderSystem.h)

add_executable(Kikan ${SOURCE_FILES})

#copy shaders to target folder
add_custom_target(copy_shaders ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Copying shaders into binary directory")
add_dependencies(Kikan copy_shaders)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link GLFW
find_library(LIB_GLFW
        NAMES libglfw3
        PATHS ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(Kikan ${LIB_GLFW})

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Kikan glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(Kikan opengl32)
endif(WIN32)