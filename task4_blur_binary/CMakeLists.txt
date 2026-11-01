cmake_minimum_required(VERSION 3.10)
project(task4_blur_binary)

find_package(OpenCV REQUIRED)

add_executable(task4_blur_binary main.cpp)
target_link_libraries(task4_blur_binary PRIVATE ${OpenCV_LIBS})

target_compile_definitions(task4_blur_binary PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")