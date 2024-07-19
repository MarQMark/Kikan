project("KikanDefInput")

#add all source files
set(SOURCE_FILES
        DefInput.cpp
)

add_library(KikanDefInput ${SOURCE_FILES})

target_include_directories(KikanDefInput
        PUBLIC ../../../include
        PUBLIC ../../../external/glfw/include
)
