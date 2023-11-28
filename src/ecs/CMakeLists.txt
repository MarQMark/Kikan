project("KikanECS")

#add all source files
set(SOURCE_FILES
        ECS.cpp
        Scene.cpp
        Entity.cpp
        TypeRegistry.cpp
        EcsThread.cpp

        systems/PhysicsSystem.cpp
        systems/SpriteRenderSystem.cpp
        systems/BasicAABBCollisionSystem.cpp
)

add_library(KikanECS ${SOURCE_FILES})

#link Kikan
target_link_libraries(KikanECS Kikan)

target_include_directories(KikanECS
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../external/glfw/include
)
