
CXX  = g++
CC   = $(CXX)
BASE = $(PWD)

CXXFLAGS = -O
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = -Wall

SRCDIR  = src

BUILD   = $(wildcard *.cpp)
SRCS	= $(BUILD) $(wildcard $(SRCDIR)/*.cpp)
HDRS 	= $(wildcard *.hpp) $(wildcard $(SRCDIR)/*.hpp)
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=)

LIBS	= -lrootfinding
LDLIBS	= -Llib $(LIBS)

.PHONY : all clean clan-all depend library

DEPEND= makedepend

all: library $(EXEC)
	@echo Compilation completed. Enjoy!

doc: Doxyfile $(SRCS) $(HDRS)
	doxygen Doxyfile && touch doc

library:
	cd $(SRCDIR) &&	$(MAKE)

clean:
	for dustfile in $(OBJS) $(EXEC); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done
	cd $(SRCDIR) && $(MAKE) clean

clean-all: clean
	-rm doc/ -rf

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)
	cd $(SRCDIR) &&	$(MAKE) depend

# DO NOT DELETE THIS LINE -- make depend depends on it.

bn.o: bn.hpp src/bisection.hpp src/iterativeMethod.hpp src/rootfinding.hpp
bn.o: src/newton.hpp src/robust.hpp src/bisection.hpp src/newton.hpp
src/bisection.o: src/bisection.hpp
src/iterativeMethod.o: src/iterativeMethod.hpp src/rootfinding.hpp
src/newton.o: src/newton.hpp
src/robust.o: src/robust.hpp src/iterativeMethod.hpp src/rootfinding.hpp
src/robust.o: src/bisection.hpp src/newton.hpp
