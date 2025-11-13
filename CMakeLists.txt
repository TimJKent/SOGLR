cmake_minimum_required(VERSION 3.11)
project(SOGLR)

set(CMAKE_CXX_STANDARD 17)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Global optimization settings that will affect all targets including Assimp
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/O2 /Oi /Ot /GL)
        add_link_options(/LTCG)
        add_compile_definitions(NDEBUG)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
    message(STATUS "Release build - optimizations enabled")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(/Od /Zi)
    else()
        add_compile_options(-O0 -g)
    endif()
    message(STATUS "Debug build - optimizations disabled")
endif()

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

add_library(SOGLR STATIC src/dummy.cpp src/stb_image.cpp)

if(WIN32)
    set(SYSTEM_LIBRARIES
        opengl32
    )
elseif(UNIX)
    set(SYSTEM_LIBRARIES
        GL
        pthread
    )
endif()

target_link_libraries(SOGLR PUBLIC
    ${SYSTEM_LIBRARIES}
    glfw
    glad
    glm
    assimp
)





target_include_directories(SOGLR PUBLIC
    ${PROJECT_SOURCE_DIR}/src/
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/include/SOGLR
    ${PROJECT_SOURCE_DIR}/
    ${PROJECT_SOURCE_DIR}/build/_deps/glm-src/glm
)