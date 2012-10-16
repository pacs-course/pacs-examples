CXX		= g++
CC		= $(CXX)
BASE	= $(PWD)

CXXFLAGS = -Wall -g
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = 

SRCS	= $(wildcard *.cpp)
HDRS	= $(wildcard *.hpp)
OBJS	= $(SRCS:.cpp=.o)
EXEC	= bn-allinone

.PHONY : clean clean-all depend

DEPEND= makedepend $(CFLAGS)


$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
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
