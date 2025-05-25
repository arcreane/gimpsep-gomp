cmake_minimum_required(VERSION 3.20)
project(Gomp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES "src/*.cpp")
include_directories(${CMAKE_SOURCE_DIR}/includes)
add_executable(Gomp ${SOURCES}
        includes/FileHelper.h
        src/FileHelper.cpp)

target_link_libraries(Gomp PRIVATE ${OpenCV_LIBS})