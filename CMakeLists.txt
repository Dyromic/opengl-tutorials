cmake_minimum_required(VERSION 3.16)

project(opengl-tutorials
    LANGUAGES CXX
    VERSION 1.0.0
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/engine")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")