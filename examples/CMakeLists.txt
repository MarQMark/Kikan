project(Kikan)

add_executable(example main.cpp)

target_include_directories(example
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include
)

target_link_directories(${PROJECT_NAME} PRIVATE external/glfw/src)

#link Kikan
target_link_libraries(example Kikan)
# Link Kikan Renderer
target_link_libraries(Kikan KikanRenderer)
# Link Kikan ECS
target_link_libraries(Kikan KikanECS)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(example glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(example opengl32)
endif(WIN32)