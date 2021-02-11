cmake_minimum_required(VERSION 3.16)
project(opengl-basic-1
    VERSION 1.0.0
    LANGUAGES CXX
)

set(EXECUTABLE_NAME "1-basic")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SOURCES 
    "${SRC_DIR}/main.cpp"
)

add_executable(${EXECUTABLE_NAME})
target_sources(${EXECUTABLE_NAME} PUBLIC ${SOURCES})
target_compile_features(${EXECUTABLE_NAME} PUBLIC cxx_std_20)

target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC engine
)

target_include_directories(${EXECUTABLE_NAME}
    PUBLIC engine
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)