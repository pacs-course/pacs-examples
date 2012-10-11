BASE	= $(PWD)
CC   	= g++

CXXFLAGS = -Wall -g
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = -O

SRCS    = $(wildcard *.cpp)
HDRS 	= $(wildcard *.hpp)
OBJS 	= $(SRCS:.cpp=.o)
EXEC	= $(OBJS:.o=)
OUT     = *.xy *.eps

.PHONY : all clean clean-all plots

all: $(EXEC) $(OBJS)
	@echo Compilation completed. Enjoy!

doc: Doxyfile $(SRCS) $(HDRS)
	doxygen Doxyfile && touch doc

plots: plot.gnu fin.xy plot.convergence.gnu fin.convergence.xy
	test -d $@ || mkdir $@
	gnuplot plot.gnu
	gnuplot plot.convergence.gnu
	touch plots

clean:
	for dustfile in $(OBJS) $(EXEC) $(OUT); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all: clean
	rm doc/ -rf
	rm plots/ -rf

