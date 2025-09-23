cmake_minimum_required(VERSION 3.11)
project(FrameGen)

set(CMAKE_CXX_STANDARD 17)

### BEGIN DEPS ###

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
include(cmake/glfw.cmake)
# GLM
include(cmake/glm.cmake)

### END DEPS ###

# Add glad static library
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

add_executable(FrameGen src/main.cpp)

file(GLOB_RECURSE FRAMEGEN_SOURCES src/*.cpp src/*.c)
target_sources(FrameGen PRIVATE ${FRAMEGEN_SOURCES})
target_link_libraries(FrameGen PRIVATE glfw opengl32 glad glm::glm)
target_include_directories(FrameGen PUBLIC
    ${PROJECT_SOURCE_DIR}/src/
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/
)