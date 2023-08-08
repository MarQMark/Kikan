project("Kikan")

#add all source files
set(SOURCE_FILES
        Engine.cpp
        core/Logging.cpp

        ecs/Scene.cpp
        ecs/Entity.cpp
        ecs/systems/PhysicsSystem.cpp
        ecs/TypeRegistry.cpp
        ecs/systems/SpriteRenderSystem.cpp

        input/Input.cpp)

add_library(Kikan ${SOURCE_FILES})

target_include_directories(Kikan
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)