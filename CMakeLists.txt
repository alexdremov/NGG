cmake_minimum_required(VERSION 3.17)
project(NGG)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -fsanitize=address")

add_subdirectory(CppSwiftyList)
include_directories(NGG)
include_directories(CppSwiftyList/)

add_executable(NGGExample Example/main.cpp)