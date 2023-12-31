project("KikanUI")

#add all source files
set(SOURCE_FILES
        UI.cpp
)

add_library(KikanUI ${SOURCE_FILES})

#link Kikan
target_link_libraries(KikanUI Kikan)

target_include_directories(KikanUI
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)