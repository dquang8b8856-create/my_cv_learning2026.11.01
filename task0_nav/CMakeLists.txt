cmake_minimum_required(VERSION 3.10)
project(task0_nav)

find_package(OpenCV REQUIRED)

add_executable(task0_nav main.cpp)
target_link_libraries(task0_nav PRIVATE ${OpenCV_LIBS})

target_compile_definitions(task0_nav PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")