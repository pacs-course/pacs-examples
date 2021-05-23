cmake_minimum_required(VERSION 3.5)

project(TestStudentLib)

# Generate executable.
...

# Include directory with header files.
...

# Set library version to use.
set(LIB_VERSION "1.0" CACHE STRING "Library version to use.")

if(${LIB_VERSION} STREQUAL "2.0")
  # Define preprocessor macro.
  ...

  # Link to proper library version.
  ...
else()
  ...
endif()

# WARNING: modify LD_LIBRARY_PATH accordingly before execution, or pass a proper "-Wl,-rpath" flag.
