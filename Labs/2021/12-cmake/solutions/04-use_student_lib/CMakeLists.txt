cmake_minimum_required(VERSION 3.5)

project(TestStudentLib)

# Generate executable.
add_executable(test_student_lib test.cpp)

# Include directory with header files.
target_include_directories(test_student_lib PUBLIC ${CMAKE_SOURCE_DIR}/../03-student_lib/src)

# Set library version to use.
set(LIB_VERSION "1.0" CACHE STRING "Library version to use.")

if(${LIB_VERSION} STREQUAL "2.0")
  # Define preprocessor macro.
  target_compile_definitions(test_student_lib PRIVATE STUDENT_VERSION_2)

  # Link to proper library version.
  target_link_libraries(test_student_lib ${CMAKE_SOURCE_DIR}/../03-student_lib/build/libstudent.so.2.0)
else()
  target_link_libraries(test_student_lib ${CMAKE_SOURCE_DIR}/../03-student_lib/build/libstudent.so.1.0)
endif()

# WARNING: modify LD_LIBRARY_PATH accordingly before execution, or pass a proper "-Wl,-rpath" flag.
