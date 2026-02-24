cmake_minimum_required(VERSION 3.20)
project(RK45 LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsExample.cmake)

pacs_setup_defaults()
set(CMAKE_CXX_STANDARD 11)

pacs_add_example_dir(LIB_NAME rk45)
