cmake_minimum_required(VERSION 3.10)
project(task3_student)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  add_compile_options(/utf-8)
endif()

add_executable(task3_student main.cpp)