BASE = $(PWD)
# CXX  = g++
CXX  = clang++

CC   = $(CXX)

CXXFLAGS = -O0 -g
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -On      attemp code optimizations

SRCS = $(wildcard *.cpp)
HDRS = $(wildcard *.hpp)
OBJS = $(SRCS:.cpp=.o)
EXEC = $(OBJS:.o=)

.PHONY : all clean clean-all depend

all: $(OBJS) $(EXEC)
	@echo Compilation completed. Enjoy!

doc: Doxyfile $(SRCS) $(HDRS)
	doxygen Doxyfile && touch doc

clean:
	for dustfile in $(OBJS) $(EXEC); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all: clean
	rm doc/ -rf

