cmake_minimum_required(VERSION 3.11)
include(FetchContent)

# Simply download GLM without processing its CMakeLists.txt
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
	FetchContent_Populate(glm)
endif()

message(STATUS "GLM source directory: ${glm_SOURCE_DIR}")

# Create interface library for GLM (header-only)
if(NOT TARGET glm)
	add_library(glm INTERFACE)
	target_include_directories(glm INTERFACE ${glm_SOURCE_DIR})
endif()