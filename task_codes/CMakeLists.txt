cmake_minimum_required(VERSION 3.10)
project(task_codes)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  add_compile_options(/utf-8)
endif()

# 1. STL 排序与去重
add_executable(stl 01_stl.cpp)

# 2. 字符串处理
add_executable(string_task 02_string.cpp)

# 3. 类与对象进阶
add_executable(student 03_student.cpp)