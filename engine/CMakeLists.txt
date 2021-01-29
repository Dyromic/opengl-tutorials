cmake_minimum_required(VERSION 3.16)
project(opengl-engine
    VERSION 1.0.0
    LANGUAGES CXX
)

set(ENGINE_NAME "engine")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES 
    "${SRC_DIR}/engine.cpp"
)

add_library(${ENGINE_NAME})
target_sources(${ENGINE_NAME} PUBLIC ${SOURCES})
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)

# GLFW options
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glad")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glm")

target_link_libraries(${ENGINE_NAME}
    PUBLIC glad::glad
    PUBLIC glfw
    PUBLIC glm::glm
)

target_include_directories(${ENGINE_NAME}
    PUBLIC glad::glad
    PUBLIC glfw
    PUBLIC glm::glm
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)