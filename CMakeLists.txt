cmake_minimum_required(VERSION 3.30)
project(gimpsep-gomp)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(gimpsep-gomp main.cpp)

target_link_libraries(gimpsep-gomp ${OpenCV_LIBS})