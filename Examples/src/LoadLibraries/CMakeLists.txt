cmake_minimum_required(VERSION 3.20)
project(LoadLibraries LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsExample.cmake)

pacs_setup_defaults()

add_library(Factory SHARED libFactory.cpp)
pacs_apply_common(Factory)

add_library(lib1 SHARED lib1.cpp)
set_target_properties(lib1 PROPERTIES OUTPUT_NAME lib1)
pacs_apply_common(lib1)

add_library(lib2 SHARED lib2.cpp)
set_target_properties(lib2 PROPERTIES OUTPUT_NAME lib2)
pacs_apply_common(lib2)

set(PACS_USE_DL ON)
set(PACS_EXTRA_LIBS Factory)

pacs_add_example_dir(LIB_NAME loadlibs)
