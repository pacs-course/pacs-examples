CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(Clustering)

SET(CMAKE_CXX_COMPILER clang++)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

SET(SRCS main_clustering.cpp Clustering.hpp Geo.hpp Distance.hpp)

ADD_EXECUTABLE(main_clustering ${SRCS})

INCLUDE_DIRECTORIES("/opt/eigen")

