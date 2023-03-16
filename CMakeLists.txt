cmake_minimum_required(VERSION 3.22)

project(FlappyDunkBot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64")

find_package(OpenCV REQUIRED)

find_library(CORE_GRAPHICS_FRAMEWORK CoreGraphics)
find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)

add_executable(hello-opencv hello-opencv.cpp WindowCaptureMac.cpp WindowCaptureMac.h)

target_link_libraries(hello-opencv ${OpenCV_LIBS} ${CORE_GRAPHICS_FRAMEWORK} ${CORE_FOUNDATION_FRAMEWORK})