BASE	= $(PWD)
CPP		= g++

CXXFLAGS = -Wall -g
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

SRCS	= $(wildcard *.cpp)
HDRS	= $(wildcard *.hpp)
OBJS	= $(SRCS:.cpp=.o)

LIBDIR  = $(PWD)/../lib

LIBNAME = $(LIBDIR)/librootfinding.a

DEPEND= makedepend $(CFLAGS)

.PHONY : all clean depend

all: $(LIBNAME)($(OBJS))

clean:
	for dustfile in $(OBJS); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)

# DO NOT DELETE

bisection.o: bisection.hpp rootfinding.hpp
newton.o: newton.hpp rootfinding.hpp
robust.o: robust.hpp rootfinding.hpp bisection.hpp newton.hpp
