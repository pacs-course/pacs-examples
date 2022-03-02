BASE	= $(PWD)
CC   	= g++

CXXFLAGS = -g -I/sw/include
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = 

SRCDIR  = src

BUILD   = $(wildcard *.cpp)
SRCS	= $(BUILD) $(wildcard $(SRCDIR)/*.cpp)
HDRS 	= $(wildcard *.hpp) $(wildcard $(SRCDIR)/*.hpp)
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=)

LIBS	= -lmesh
LDLIBS	= -Llib $(LIBS)

.PHONY : all clean clean-all depend library

DEPEND = makedepend

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

main.o: src/mesh.hpp src/shape.hpp src/triangle.hpp src/quadrangle.hpp
src/mesh.o: src/mesh.hpp src/shape.hpp src/triangle.hpp src/quadrangle.hpp
src/quadrangle.o: src/quadrangle.hpp src/shape.hpp
src/shape.o: src/shape.hpp
src/triangle.o: src/triangle.hpp src/shape.hpp
