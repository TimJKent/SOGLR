cmake_minimum_required(VERSION 3.10)
project(SOGLR_Examples)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable C++23 features for MSVC
if(MSVC)
    add_compile_options(/std:c++latest)
endif()

add_subdirectory(.. SOGLR)
include_directories(../include)
add_executable(SOGLR_Examples 7_RenderText.cpp)

target_link_libraries(SOGLR_Examples PRIVATE 
    SOGLR
)