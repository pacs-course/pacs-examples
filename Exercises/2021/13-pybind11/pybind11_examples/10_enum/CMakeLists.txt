cmake_minimum_required(VERSION 2.8.12)
project(example)

set(CMAKE_CXX_STANDARD 14)

find_package(pybind11 REQUIRED HINTS $ENV{mkPybind11Prefix})
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})
pybind11_add_module(example example.cpp)
