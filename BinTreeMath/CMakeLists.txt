cmake_minimum_required(VERSION 3.26)
project(BinTreeMath)

set(CMAKE_CXX_STANDARD 17)

add_executable(BinTreeMath main.cpp
        libs/MathsFunctions.cpp
        libs/MathsFunctions.h)
