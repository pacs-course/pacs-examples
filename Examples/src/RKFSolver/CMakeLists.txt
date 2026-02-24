cmake_minimum_required(VERSION 3.20)
project(RKFSolver LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsExample.cmake)

pacs_setup_defaults()
set(PACS_LINK_LIB_NAMES Newton)

pacs_add_example_dir(LIB_NAME RKF)
