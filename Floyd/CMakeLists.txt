cmake_minimum_required(VERSION 3.26)
project(Floyd)

set(CMAKE_CXX_STANDARD 17)

add_executable(Floyd main.cpp
        libs/MathsFunctions.cpp)
