cmake_minimum_required(VERSION 3.10)
project(vision_demo)

find_package(OpenCV REQUIRED)

add_executable(vision_demo main.cpp)
target_link_libraries(vision_demo PRIVATE ${OpenCV_LIBS})
target_compile_definitions(vision_demo PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")