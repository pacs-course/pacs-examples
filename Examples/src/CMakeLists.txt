cmake_minimum_required(VERSION 3.20)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/PacsCommon.cmake)

pacs_setup_defaults()

# Add all subdirectories that contain a CMakeLists.txt
file(GLOB CHILD_DIRS RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
foreach(child IN LISTS CHILD_DIRS)
  if(IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${child}")
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/${child}/CMakeLists.txt")
      add_subdirectory(${child})
    endif()
  endif()
endforeach()
