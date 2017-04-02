
CXX=clang++
CC=$(CXX)
CXXFLAGS=-O3

LIBNAME=liblegendre_rule.a
SRCS=legendre_rule.cpp
OBJS=$(SRCS:.cpp=.o)

.phony: all clean

all: $(LIBNAME)($(OBJS))

test: $(OBJS)

clean:
	$(RM) *.o *~ $(LIBNAME) test

