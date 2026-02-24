cmake_minimum_required(VERSION 3.20)
project(basicZeroFun LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsExample.cmake)

pacs_setup_defaults()

pacs_add_example_dir(PYTHON_MODULE_NAME zeroFun EXPLICIT_OTHERS pyZeroFun.cpp)
