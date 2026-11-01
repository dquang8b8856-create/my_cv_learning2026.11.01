cmake_minimum_required(VERSION 3.10)
project(task1_stl)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  add_compile_options(/utf-8)
endif()

add_executable(task1_stl main.cpp)