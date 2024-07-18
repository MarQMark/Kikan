project("KikanDefRenderer")

#add all source files
set(SOURCE_FILES
        ../../glew/glew.c

        DefRenderer.cpp
)

add_library(KikanDefRenderer SHARED ${SOURCE_FILES})

target_include_directories(KikanDefRenderer
        PUBLIC ../../../include
        PUBLIC ../../../external/glfw/include
)
target_link_directories(KikanDefRenderer PRIVATE external/glfw/src)

# Link Kikan Renderer
target_link_libraries(KikanDefRenderer glfw)

#define GLEW_STATIC (necessary for glew)
add_compile_definitions(GLEW_STATIC)

#link all necessary libraries for opengl
if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(KikanDefRenderer glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(KikanDefRenderer opengl32)
endif(WIN32)