project("KikanStdInput")

#add all source files
set(SOURCE_FILES
        StdInput.cpp
)

add_library(KikanStdInput ${SOURCE_FILES})

target_include_directories(KikanStdInput
        PUBLIC ../../../include
        PUBLIC ../../../external/glfw/include
)
