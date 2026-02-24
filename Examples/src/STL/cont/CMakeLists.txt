cmake_minimum_required(VERSION 3.20)
project(cont LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/PacsExample.cmake)

pacs_setup_defaults()

pacs_add_example_dir(EXPLICIT_MAINS deque1.cpp multimap1.cpp multiset1.cpp oddeven.cpp ostream_iterator.cpp set1.cpp setcmp1.cpp unordset1.cpp)
