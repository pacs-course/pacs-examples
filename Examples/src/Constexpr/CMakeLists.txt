cmake_minimum_required(VERSION 3.20)
project(Constexpr LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsExample.cmake)

pacs_setup_defaults()

pacs_add_example_dir()
