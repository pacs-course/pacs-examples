cmake_minimum_required(VERSION 3.20)
project(AllDynamic LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsExample.cmake)

pacs_setup_defaults()
set(PACS_USE_MUPARSER ON)

pacs_add_example_dir(LIB_NAME AdamsRules)
