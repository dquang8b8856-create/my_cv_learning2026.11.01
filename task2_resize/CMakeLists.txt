cmake_minimum_required(VERSION 3.10)
project(task2_resize)

find_package(OpenCV REQUIRED)

add_executable(task2_resize main.cpp)
target_link_libraries(task2_resize PRIVATE ${OpenCV_LIBS})

target_compile_definitions(task2_resize PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")