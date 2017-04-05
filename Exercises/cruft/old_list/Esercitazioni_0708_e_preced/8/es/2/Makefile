BASE	= $(PWD)
CC   	= g++

CXXFLAGS = -g
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

LIBS	= -lfixpt
LDLIBS	= -Llib $(LIBS)

.PHONY : all clean clean-all depend library

DEPEND = makedepend

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
	$(DEPEND) -Y $(SRCS)
	cd $(SRCDIR) &&	$(MAKE) depend

# DO NOT DELETE

main.o: src/fpnewton.hpp src/fpbase.hpp src/fpsolve.hpp lib/libfixpt.a
src/fpdummy.o: src/fpdummy.hpp src/fpbase.hpp
src/fpnewton.o: src/fpnewton.hpp src/fpbase.hpp
