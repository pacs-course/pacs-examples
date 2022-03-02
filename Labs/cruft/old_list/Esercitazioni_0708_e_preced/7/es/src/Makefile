BASE	= $(PWD)
CPP   	= g++

CXXFLAGS = -g -I/sw/include
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

SRCS	= $(wildcard *.cpp)
HDRS 	= $(wildcard *.hpp)
OBJS 	= $(SRCS:.cpp=.o)

LIBDIR  = $(PWD)/../lib

LIBNAME = $(LIBDIR)/libmesh.a

DEPEND= makedepend $(CFLAGS)

.PHONY : default clean depend

default: $(LIBNAME)($(OBJS))
	ranlib $(LIBNAME)

clean:
	for dustfile in $(OBJS); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)

# DO NOT DELETE

mesh.o: mesh.hpp shape.hpp triangle.hpp quadrangle.hpp
quadrangle.o: quadrangle.hpp shape.hpp
shape.o: shape.hpp
triangle.o: triangle.hpp shape.hpp
