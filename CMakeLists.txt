cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project (DrawingTriangles)

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# OpenGL (set policy for GLVND)
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

# GLM
find_package(glm REQUIRED)

# GLFW
find_package(glfw3 REQUIRED)

# GLAD
add_library(glad vendor/glad/include/glad/glad.h vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Link up the libraries
set(LIBS glad OpenGL glfw)
include_directories("${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${PROJECT_NAME} ${LIBS})