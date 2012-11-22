
CXX = clang++
#CXX = g++-4.7
CC  = $(CXX)

CXXFLAGS = -Wall -ansi -pedantic -g -std=c++11

TARGET = main_randomlap

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

DEPEND = make.dep

all: $(DEPEND) $(TARGET)

$(TARGET): $(OBJS)

clean:
	/bin/rm $(DEPEND) $(OBJS)

-include make.dep

$(DEPEND):
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > $@

