cmake_minimum_required(VERSION 3.13)
project(OpenGLTutorial)
set(CMAKE_CXX_STANDARD 11)

include_directories("./inc/")

link_directories("./lib/")

set(SRC_LIST main.cpp glad.c)

add_executable(OpenGLTutorial ${SRC_LIST})

target_link_libraries(OpenGLTutorial glfw3)