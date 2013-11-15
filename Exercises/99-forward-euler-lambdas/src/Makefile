
CXX  = g++
CC   = $(CXX)

CXXFLAGS = -O -std=gnu++11
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = -Wall

SRCDIR  = src

BUILD   = $(wildcard *.cpp)
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=)

.PHONY : all clean clan-all depend 

all: $(EXEC)
	@echo Compilation completed. Enjoy!

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $@.o -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	for dustfile in $(OBJS) $(EXEC); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all: clean


