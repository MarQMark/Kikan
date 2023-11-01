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
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../external/glfw/include
)
