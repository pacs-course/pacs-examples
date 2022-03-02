BASE	= $(PWD)
CC   	= g++

INCLS = -I$(PWD)/..

CXXFLAGS = -g $(INCLS)
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

LIBS	= -lnumint
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
	-rm doc/ lib/ -rf

depend: $(SRCS)
	$(DEPEND) -Y. $(SRCS)
	cd $(SRCDIR) &&	$(MAKE) depend

# DO NOT DELETE

main_integration.o: src/numerical_integration.hpp src/numerical_rule.hpp lib/libnumint.a
src/numerical_integration.o: src/numerical_integration.hpp
src/numerical_integration.o: src/numerical_rule.hpp
src/numerical_rule.o: src/numerical_rule.hpp
