cmake_minimum_required(VERSION 3.10)
project(vision_demo)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  add_compile_options(/utf-8)
endif()

find_package(OpenCV REQUIRED)

add_executable(vision_demo
  main.cpp
  src/demo_version.cpp
  src/demo_basic.cpp
  src/demo_roi.cpp
  src/demo_resize.cpp
  src/demo_hsv.cpp
  src/demo_blurbinary.cpp
  src/demo_canny.cpp
  src/demo_components.cpp
)

target_include_directories(vision_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(vision_demo PRIVATE ${OpenCV_LIBS})
target_compile_definitions(vision_demo PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/results)