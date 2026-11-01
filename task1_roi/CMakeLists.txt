cmake_minimum_required(VERSION 3.10)
project(task1_roi)

find_package(OpenCV REQUIRED)

add_executable(task1_roi main.cpp)
target_link_libraries(task1_roi PRIVATE ${OpenCV_LIBS})

target_compile_definitions(task1_roi PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")