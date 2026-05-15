cmake_minimum_required(VERSION 3.18)
project(pacs_nanobind LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python 3.9 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(Eigen3 CONFIG QUIET)

Python_add_library(pacs_nb MODULE WITH_SOABI
    bindings_nanobind.cpp
    src/nb_combined.cpp
)

target_compile_options(pacs_nb PRIVATE -fvisibility=hidden -fno-strict-aliasing)
target_include_directories(pacs_nb PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (Eigen3_FOUND)
    target_link_libraries(pacs_nb PRIVATE Eigen3::Eigen)
else()
    target_include_directories(pacs_nb PRIVATE /usr/include/eigen3)
endif()
