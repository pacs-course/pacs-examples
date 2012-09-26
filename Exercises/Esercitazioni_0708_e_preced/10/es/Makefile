BASE	= $(PWD)
CC   	= g++

CXXFLAGS = -O -DNDEBUG -Wall
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

.PHONY : all clean clean-all depend

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

main.o: src/cg.hpp src/solver_policies.hpp src/solver_policies.hpp
