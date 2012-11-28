cmake_minimum_required(VERSION 2.8)

# set project name to ExpressionTemplate
project(ET)

set(ET_VERSION_MAJOR 1)
set(ET_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

add_subdirectory(include)

add_executable(main_integration
  main_integration.cpp
  ${INCLUDES}
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  # configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif()
