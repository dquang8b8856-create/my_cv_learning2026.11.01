cmake_minimum_required(VERSION 3.10)
project(task5_canny)

find_package(OpenCV REQUIRED)

add_executable(task5_canny main.cpp)
target_link_libraries(task5_canny PRIVATE ${OpenCV_LIBS})

target_compile_definitions(task5_canny PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")