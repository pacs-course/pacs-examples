cmake_minimum_required(VERSION 3.5)

project(StudentLib)

# Enable optimizations.
set(CMAKE_BUILD_TYPE Release)

# Set library version to use.
set(LIB_VERSION "1.0" CACHE STRING "Library version to use.")

# Generate shared library from source files.
if(${LIB_VERSION} STREQUAL "1.0")
  # Generate library.
  add_library(student SHARED src/Student_v1.cpp)

  # Set library version.
  set_target_properties(student PROPERTIES VERSION 1.0)
elseif(${LIB_VERSION} STREQUAL "2.0")
  add_library(student SHARED src/Student_v2.cpp)
  set_target_properties(student PROPERTIES VERSION 2.0)
else()
  message(FATAL_ERROR "Wrong library version specified.")
endif()

# Include directory with header files.
target_include_directories(student PUBLIC src)
