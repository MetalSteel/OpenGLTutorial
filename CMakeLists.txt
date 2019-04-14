cmake_minimum_required(VERSION 3.13)
project(OpenGLTutorial)
set(CMAKE_CXX_STANDARD 11)

include_directories("${PROJECT_SOURCE_DIR}/inc/" "${PROJECT_SOURCE_DIR}/src/include")

link_directories("${PROJECT_SOURCE_DIR}/lib/")

set(SRC_LIST
        src/source/main.cpp
        src/util/glad.c
        src/util/stb_image.cpp
        src/include/Shader.h
        src/source/Shader.cpp
        src/include/Camera.h
        src/source/Camera.cpp
        )

add_executable(OpenGLTutorial ${SRC_LIST})

target_link_libraries(OpenGLTutorial glfw3)