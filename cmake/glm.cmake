cmake_minimum_required(VERSION 3.11) # FetchContent is new in version 3.11.
include(FetchContent)
cmake_policy(SET CMP0169 OLD)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)