cmake_minimum_required (VERSION 3.0)
project (hellomake)

include_directories("${PROJECT_SOURCE_DIR}/include")
add_subdirectory(src)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")
add_executable(hellomake src/hellomake.cpp)
target_link_libraries(hellomake hellofunc)


