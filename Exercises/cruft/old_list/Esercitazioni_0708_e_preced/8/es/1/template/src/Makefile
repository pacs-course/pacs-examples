BASE	= $(PWD)
CPP   	= g++

CXXFLAGS = -g
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

SRCS	= $(wildcard *.cpp)
HDRS 	= $(wildcard *.hpp)
OBJS 	= $(SRCS:.cpp=.o)

LIBDIR  = $(PWD)/../lib

LIBNAME = $(LIBDIR)/libnumint.a

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

numerical_rule.o: numerical_rule.hpp
