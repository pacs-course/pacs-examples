# Install script for directory: /home/forma/Temp/pacs-examples/Examples/src/Utilities

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/forma/Temp/pacs-examples/Examples")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpacs.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpacs.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpacs.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/forma/Temp/pacs-examples/Examples/src/Utilities/libpacs.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpacs.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpacs.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libpacs.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/forma/Temp/pacs-examples/Examples/src/Utilities/libpacs.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/Arithmetic.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/CloningUtilities.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/Factory.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/JoinVectors.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/Proxy.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/StatisticsComputations.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/absdiff.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/booleanConcept.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/chrono.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/cxxversion.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/extendedAssert.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/functionConcepts.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/gnuplot-iostream.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/hashCombine.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/is_complex.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/is_eigen.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/is_specialization.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/overloaded.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/parallel_for.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/range_to_vector.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/readCSV.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/scientific_precision.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/setUtilities.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/stonum.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/string_utility.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/tostring.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/tuple_utilities.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/type_name.hpp"
    "/home/forma/Temp/pacs-examples/Examples/src/Utilities/GetPot"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/forma/Temp/pacs-examples/Examples/src/Utilities/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
