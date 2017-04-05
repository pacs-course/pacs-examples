BASE	= $(PWD)
CPP   	= g++

CXXFLAGS = -pg
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -pg       generate code suitable for the profiler (gprof)
# -O       attemp code optimizations
# -Wall    shows all warnings
# -pedantic shows very pedant warnings
# -funroll-loop unrolls internal loops
# -funroll-all-loops unrolls all loops

SRCS	= $(wildcard *.cpp)
HDRS 	= $(wildcard *.hpp)
OBJS 	= $(SRCS:.cpp=.o)
EXEC	= bn.monolithic

.PHONY : clean clean-all depend

LDFLAGS = -O

DEPEND= makedepend $(CFLAGS)

$(EXEC): $(OBJS)
	$(CPP) $(LDFLAGS) -o $@ $<
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

