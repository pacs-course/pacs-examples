cmake_minimum_required(VERSION 2.8.12)
project(example)

find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS})

find_package(pybind11 REQUIRED HINTS $ENV{mkPybind11Prefix})
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})
pybind11_add_module(example_06_class_numpy_eigen example.cpp)
