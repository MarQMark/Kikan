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
set(SOURCE_FILES scr/main.cpp scr/glew/glew.c scr/Engine.cpp include/Kikan/Engine.h scr/ecs/Scene.cpp include/Kikan/ecs/Scene.h scr/ecs/Entity.cpp include/Kikan/ecs/Entity.h scr/opengl/Renderer.cpp include/Kikan/opengl/Renderer.h include/Kikan/ecs/components/Physics.h include/Kikan/ecs/components/Transform.h include/Kikan/ecs/systems/ISystem.h include/Kikan/ecs/components/IComponent.h scr/ecs/systems/PhysicsSystem.cpp include/Kikan/ecs/systems/PhysicsSystem.h include/Kikan/ecs/TypeRegistry.h scr/ecs/TypeRegistry.cpp scr/opengl/Shader.cpp include/Kikan/opengl/Shader.h scr/opengl/batching/Batch.cpp include/Kikan/opengl/batching/Batch.h scr/opengl/buffers/VertexBuffer.cpp include/Kikan/opengl/buffers/VertexBuffer.h include/Kikan/opengl/vertices/IVertex.h scr/opengl/batching/AutoBatch.cpp include/Kikan/opengl/batching/AutoBatch.h scr/opengl/batching/ManuelBatch.cpp include/Kikan/opengl/batching/ManuelBatch.h include/Kikan/opengl/vertices/DefaultVertex.h include/Kikan/opengl/buffers/VertexBufferLayout.h scr/opengl/VertexRegistry.cpp include/Kikan/opengl/vertices/VertexRegistry.h scr/opengl/buffers/IndexBuffer.cpp include/Kikan/opengl/buffers/IndexBuffer.h scr/ecs/systems/SpriteRenderSystem.cpp include/Kikan/ecs/systems/SpriteRenderSystem.h include/Kikan/ecs/components/TriangleSprite.h include/Kikan/util/EarClipping.h include/Kikan/ecs/components/PolygonSprite.h include/Kikan/ecs/components/QuadSprite.h include/Kikan/ecs/systems/IRenderSystem.h scr/opengl/Camera.cpp include/Kikan/opengl/Camera.h)

add_executable(Kikan ${SOURCE_FILES})

target_include_directories(Kikan PUBLIC ${PROJECT_SOURCE_DIR}/include/Kikan)


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




############################### Kikan Lib ######################################
project(KikanLib)

add_library(KikanLib ${SOURCE_FILES})

target_include_directories(KikanLib PUBLIC ${PROJECT_SOURCE_DIR}/include/Kikan)

add_dependencies(KikanLib copy_shaders)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link GLFW
find_library(LIB_GLFW
        NAMES libglfw3
        PATHS ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(KikanLib ${LIB_GLFW})

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(KikanLib glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(KikanLib opengl32)
endif(WIN32)