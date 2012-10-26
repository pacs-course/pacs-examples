BASE = $(PWD)
CXX  = g++
CC   = $(CXX)

EIGEN_PATH = /usr/include/eigen3

CXXFLAGS = -g -Wall -I$(EIGEN_PATH)
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

SRCS	= $(wildcard *.cpp)
HDRS 	= $(wildcard *.hpp)
OBJS 	= $(SRCS:.cpp=.o)

LIBDIR  = $(PWD)/../lib

LIBNAME = $(LIBDIR)/libeigenvalues.a

DEPEND= makedepend $(CFLAGS)

.PHONY : default clean depend

default: $(LIBNAME)($(OBJS))

clean:
	for dustfile in $(OBJS); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)

# DO NOT DELETE

power.o: power.hpp
#matrix.o: matrix.hpp vector.hpp
#power.o: power.hpp linearAlgebra.hpp vector.hpp matrix.hpp
#power_orig.o: power.hpp linearAlgebra.hpp vector.hpp matrix.hpp
#vector.o: vector.hpp
