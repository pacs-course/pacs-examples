cmake_minimum_required(VERSION 3.20)
project(OptimizationAndProfiling LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsCommon.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/PacsExample.cmake)

pacs_setup_defaults()
set(CMAKE_CXX_STANDARD 20)
set(PACS_LINK_PACS ON)

pacs_add_example_dir(EXPLICIT_MAINS aliasing.cpp esempio.cpp esempioExtAss.cpp esempio_errore.cpp leak.cpp provamassif.cpp EXPLICIT_OTHERS namemangling.cpp rational.cpp)
