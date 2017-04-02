CXX = clang++
CC  = $(CXX)

CXXFLAGS = -Wall -std=c++11

TARGET = main

SRCS = main.cpp
OBJS = main.o

.phony: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS)

$(TARGET): $(OBJS)

