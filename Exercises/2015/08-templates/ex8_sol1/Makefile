
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
HDRS 	= $(wildcard *.hpp) 
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=)

.PHONY : all clean clan-all depend library

DEPEND= makedepend

all: $(EXEC)
	@echo Compilation completed. Enjoy!

doc: Doxyfile $(HDRS)
	doxygen Doxyfile && touch doc

clean:
	for dustfile in $(OBJS) $(EXEC); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all: clean
	!( test -e doc ) || rm -rf doc;

depend: $(BUILD)
	$(DEPEND) -Y. $(BUILD)

# DO NOT DELETE THIS LINE -- make depend depends on it.

