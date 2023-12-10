cmake_minimum_required(VERSION 3.8)

project(code_snip)

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)

add_executable(universal_hash template/universal_hash.cpp)
add_executable(function_traits template/function_traits.cc)
add_subdirectory(ccia)