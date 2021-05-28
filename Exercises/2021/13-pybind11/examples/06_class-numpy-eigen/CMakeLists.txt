cmake_minimum_required(VERSION 3.0.0)
project(example)

find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS} HINTS $ENV{mkEigenPrefix})

find_package(pybind11 REQUIRED HINTS $ENV{mkPybind11Prefix})
include_directories(SYSTEM ${pybind11_INCLUDE_DIRS})

pybind11_add_module(example_06_class_numpy_eigen example.cpp)
# Example that shows how to pass a value from Python to C++.
target_compile_definitions(example_06_class_numpy_eigen PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
