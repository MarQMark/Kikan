cmake_minimum_required(VERSION 3.23)
project(Kikan)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 ")
endif(UNIX)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -lmingw32")
endif(WIN32)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCE_FILES scr/main.cpp scr/glew.c)

add_executable(Kikan ${SOURCE_FILES})

add_compile_definitions(GLEW_STATIC)

find_library(LIB_GLFW
        NAMES libglfw3
        PATHS ${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(Kikan ${LIB_GLFW})

if (UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(Kikan glut GL dl X11 pthread)
endif(UNIX)

if (WIN32)
    target_link_libraries(Kikan opengl32)
endif(WIN32)