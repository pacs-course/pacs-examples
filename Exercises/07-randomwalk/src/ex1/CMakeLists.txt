CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(RandomLaplacian)

SET(CMAKE_CXX_COMPILER clang++)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -std=c++11")

SET(SRCS main_randomlap.cpp)
SET(HDRS RandomWalk.hpp Distributions.hpp)

ADD_EXECUTABLE(main_randomlap ${SRCS} ${HDRS})

INCLUDE_DIRECTORIES("/opt/eigen")

INCLUDE(CPack)

INSTALL(FILES ${SRCS} CMakeLists.txt
  DESTINATION .
)

