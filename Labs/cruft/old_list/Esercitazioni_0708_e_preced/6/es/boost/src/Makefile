BASE	= $(PWD)
CPP   	= g++

CXXFLAGS = -O -Wall -I/sw/include
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

cyclicJacobi.o: cyclicJacobi.hpp givens.hpp linearAlgebra.hpp
givens.o: givens.hpp linearAlgebra.hpp
linearAlgebra.o: linearAlgebra.hpp
power.o: power.hpp linearAlgebra.hpp
