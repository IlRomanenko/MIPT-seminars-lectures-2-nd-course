cmake_minimum_required(VERSION 3.6)
project(Solution)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D LOCAL")

set(SOURCE_FILES main.cpp)
add_executable(Solution ${SOURCE_FILES})