cmake_minimum_required(VERSION 2.8)

project(Gameboy)
enable_testing()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")

add_subdirectory(tests)
add_subdirectory(src)
