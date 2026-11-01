cmake_minimum_required(VERSION 3.10)
project(task3_hsv)

find_package(OpenCV REQUIRED)

add_executable(task3_hsv main.cpp)
target_link_libraries(task3_hsv PRIVATE ${OpenCV_LIBS})

target_compile_definitions(task3_hsv PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")