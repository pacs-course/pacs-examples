BASE	= $(PWD)
CC   	= g++

CXXFLAGS = -O -Wall -I/sw/include
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = -O

SRCDIR  = src

BUILD   = $(wildcard *.cpp)
SRCS	= $(BUILD) $(wildcard $(SRCDIR)/*.cpp)
HDRS 	= $(wildcard *.hpp) $(wildcard $(SRCDIR)/*.hpp)
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=)

LIBS	= -leigenvalues
LDLIBS	= -Llib $(LIBS)

.PHONY : all clean clean-all depend library

DEPEND= makedepend

all: library $(EXEC)
	@echo Compilation completed. Enjoy!

doc: Doxyfile $(SRCS) $(HDRS)
	doxygen Doxyfile && touch doc

library:
	( test -d lib ) || mkdir lib
	cd $(SRCDIR) &&	$(MAKE)

clean:
	for dustfile in $(OBJS) $(EXEC); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done
	cd $(SRCDIR) && $(MAKE) clean

clean-all: clean
	!( test -d lib ) || rm -rf lib
	!( test -d doc ) || rm -rf doc

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)
	cd $(SRCDIR) &&	$(MAKE) depend
# DO NOT DELETE

eig.o: src/linearAlgebra.hpp src/hilbert.hpp src/linearAlgebra.hpp
eig.o: src/cyclicJacobi.hpp src/givens.hpp src/power.hpp
src/cyclicJacobi.o: src/cyclicJacobi.hpp src/givens.hpp src/linearAlgebra.hpp
src/givens.o: src/givens.hpp src/linearAlgebra.hpp
src/linearAlgebra.o: src/linearAlgebra.hpp
src/power.o: src/power.hpp src/linearAlgebra.hpp
