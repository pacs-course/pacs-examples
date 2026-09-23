cmake_minimum_required(VERSION 3.16)
project(autodiff_demo LANGUAGES CXX)

# Prefer an Eigen installation explicitly selected by the environment.  If it
# does not provide Eigen's CMake package, fall back to the usual search paths.
if(DEFINED ENV{mkEigenHome} AND NOT "$ENV{mkEigenHome}" STREQUAL "")
    find_package(Eigen3 3.3 QUIET NO_MODULE
        HINTS "$ENV{mkEigenHome}"
        NO_DEFAULT_PATH)
endif()

if(NOT Eigen3_FOUND)
    find_package(Eigen3 3.3 REQUIRED NO_MODULE)
endif()

# autodiff is bundled with these examples and is a header-only library.
add_library(autodiff INTERFACE)
target_include_directories(autodiff INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/../../include")

add_executable(autodiff_forward_demo autodiff_demo.cpp)
target_compile_features(autodiff_forward_demo PRIVATE cxx_std_17)
target_link_libraries(autodiff_forward_demo PRIVATE autodiff Eigen3::Eigen)

add_executable(autodiff_reverse_demo autodiff_reverse_demo.cpp)
target_compile_features(autodiff_reverse_demo PRIVATE cxx_std_17)
target_link_libraries(autodiff_reverse_demo PRIVATE autodiff Eigen3::Eigen)
