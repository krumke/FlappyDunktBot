cmake_minimum_required(VERSION 3.17)

project(FlappyDunkBot)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(hello-opencv hello-opencv.cpp)

target_link_libraries(hello-opencv ${OpenCV_LIBS})
