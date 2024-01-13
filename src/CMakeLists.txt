project("Kikan")

#add all source files
set(SOURCE_FILES
        Engine.cpp
        core/Logging.cpp

        input/Input.cpp
)

add_library(Kikan ${SOURCE_FILES})

target_include_directories(Kikan
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)