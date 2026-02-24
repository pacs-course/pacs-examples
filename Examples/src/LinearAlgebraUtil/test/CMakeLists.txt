cmake_minimum_required(VERSION 3.20)
project(test LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsExample.cmake)

pacs_setup_defaults()
set(PACS_USE_OPENMP ON)
set(PACS_USE_TBB ON)

pacs_add_example_dir(EXPLICIT_MAINS testBasicMinFun.cpp testBasicZeroFun.cpp testRotatingVector.cpp test_RotatingMatrix.cpp test_mathUtils.cpp test_thomas.cpp)
