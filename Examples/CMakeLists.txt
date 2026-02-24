cmake_minimum_required(VERSION 3.20)
project(pacs_examples LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/PacsCommon.cmake)

pacs_setup_defaults()

add_subdirectory(src)
