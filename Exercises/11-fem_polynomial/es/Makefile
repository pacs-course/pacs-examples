BASE	= $(PWD)
CC	= g++

OPTFLAGS = -O3
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = -llapack
INCLS	=
CXXFLAGS= $(OPTFLAGS) $(INCLS) -Wall

SRCDIR  = src

BUILD   = $(wildcard *.cpp)
SRCS	= $(BUILD) $(wildcard $(SRCDIR)/*.cpp)
HDRS 	= $(wildcard *.hpp) $(wildcard $(SRCDIR)/*.hpp)
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=)

.PHONY : all clean clean-all depend library

DEPEND = makedepend

all: $(EXEC)
	@echo Compilation completed. Enjoy!

doc: Doxyfile $(SRCS) $(HDRS)
	doxygen Doxyfile && touch doc

clean:
	for dustfile in $(OBJS) $(EXEC); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all: clean
	-rm doc/ -rf

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)

# DO NOT DELETE

main.o: src/pacsfem.hpp src/polynomial.hpp src/pacsfem.hpp
main.o: src/linearAlgebra.hpp src/linearSolver.hpp src/gauss.hpp
main.o: src/eigenSolver.hpp src/eigenSolver.hpp
