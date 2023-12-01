cmake_minimum_required(VERSION 3.8)

project(code_snip)

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)

add_executable(universal_hash universal_hash.cpp)
add_subdirectory(CCIA)