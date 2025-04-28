cmake_minimum_required(VERSION 3.20)
project(Gomp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES "*.cpp")

add_executable(Gomp ${SOURCES})

target_link_libraries(Gomp PRIVATE ${OpenCV_LIBS})