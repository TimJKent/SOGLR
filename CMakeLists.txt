cmake_minimum_required(VERSION 3.11)
project(SOGLR)

set(CMAKE_CXX_STANDARD 17)

### BEGIN DEPS ###

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
include(cmake/glfw.cmake)
# GLM
include(cmake/glm.cmake)
# Assimp
include (cmake/assimp.cmake)

### END DEPS ###

# Add glad static library
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

add_library(SOGLR STATIC src/dummy.cpp)
target_link_libraries(SOGLR PUBLIC glfw opengl32 glad glm::glm assimp)
target_include_directories(SOGLR PUBLIC
    ${PROJECT_SOURCE_DIR}/src/
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/build/_deps/glm-src/glm
)