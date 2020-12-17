cmake_minimum_required(VERSION 3.17)
project(NGG)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")

add_subdirectory(CppSwiftyList)
include_directories(NGG)
include_directories(CppSwiftyList/)
add_executable(ngg NGG/ngg.cpp)
add_executable(nggload NGG/nggload.cpp)

add_custom_command(TARGET ngg
        # Run after all other rules within the target have been executed
        POST_BUILD
        COMMAND cp ngg /usr/local/bin/ngg
        COMMENT "Installing ngg"
        VERBATIM
        )

add_custom_command(TARGET nggload
        # Run after all other rules within the target have been executed
        POST_BUILD
        COMMAND cp nggload /usr/local/bin/nggload
        COMMENT "Installing nggload"
        VERBATIM
        )