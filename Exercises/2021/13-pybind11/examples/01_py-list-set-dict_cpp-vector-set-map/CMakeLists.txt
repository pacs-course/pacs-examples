cmake_minimum_required(VERSION 3.0.0)
project(example)

find_package(pybind11 REQUIRED HINTS $ENV{mkPybind11Prefix})
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})

pybind11_add_module(example example.cpp)
