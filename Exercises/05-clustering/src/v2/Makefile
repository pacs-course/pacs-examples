
CXX = clang++
#CXX = g++-4.7
CC  = $(CXX)

EIGEN_DIR=/usr/local/include/eigen3

INCLUDES=-I$(EIGEN_DIR)

CXXFLAGS = -Wall -ansi -pedantic -std=c++11 $(INCLUDES)

TARGET = main_clustering

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

DEPEND = make.dep

all: $(DEPEND) $(TARGET) 

$(TARGET): $(OBJS)

clean:
	/bin/rm $(DEPEND) $(OBJS)

-include $(DEPEND)

$(DEPEND):
	g++ -std=c++0x -MM $(SRCS) > $@

