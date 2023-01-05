project(Kikan)

add_executable(example main.cpp)

target_include_directories(example
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

#copy shaders to target folder
add_custom_target(copy_shaders_examples ALL
        COMMAND  ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders
        ${PROJECT_BINARY_DIR}/shaders
        COMMENT "Copying shaders into examples binary directory")
add_dependencies(example copy_shaders_examples)

#link Kikan
target_link_libraries(example Kikan)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link GLFW
find_library(LIB_GLFW
        NAMES libglfw3
        PATHS ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(example ${LIB_GLFW})

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(example glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(example opengl32)
endif(WIN32)