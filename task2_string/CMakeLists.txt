cmake_minimum_required(VERSION 3.10)
project(task2_string)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  add_compile_options(/utf-8)
endif()

add_executable(task2_string main.cpp)