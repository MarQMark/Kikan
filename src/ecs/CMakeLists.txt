project("KikanECS")

#add all source files
set(SOURCE_FILES
        ECS.cpp
        Scene.cpp
        Entity.cpp
        TypeRegistry.cpp
        systems/PhysicsSystem.cpp
        systems/SpriteRenderSystem.cpp
        )

add_library(KikanECS ${SOURCE_FILES})

target_include_directories(KikanECS
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

#copy shaders to target folder
#add_custom_target(copy_shaders ALL
#        COMMAND  ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_CURRENT_SOURCE_DIR}/../../shaders
#        ${PROJECT_BINARY_DIR}/shaders
#        COMMENT "Copying shaders into binary directory")
#add_dependencies(KikanRenderer copy_shaders)
#
##define GLEW_STATIC (necessary for glew)
#add_compile_definitions(GLEW_STATIC)
#
##link GLFW
#find_library(LIB_GLFW
#        NAMES libglfw3.a
#        PATHS ${PROJECT_SOURCE_DIR}/../lib)
#target_link_libraries(KikanRenderer ${LIB_GLFW})
#
##link all necessary libraries for opengl
#if (UNIX)
#    find_package(OpenGL REQUIRED)
#    target_link_libraries(KikanRenderer glut GL dl X11 pthread)
#endif(UNIX)
#
#if (WIN32)
#    target_link_libraries(KikanRenderer opengl32)
#endif(WIN32)