cmake_minimum_required(VERSION 3.26)
project(SHA256)

set(CMAKE_CXX_STANDARD 17)

add_executable(SHA256 main.cpp
        Sha256.cpp
        Sha256.h)
