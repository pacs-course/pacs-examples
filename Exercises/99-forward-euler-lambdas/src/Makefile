
CXX  ?= g++
CC   ?= $(CXX)
MKOCTFILE ?= mkoctfile

CXXFLAGS ?= -O -std=gnu++11
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

LDFLAGS = -Wall

SRCDIR  = src

BUILD   = $(wildcard *.cpp)
OBJS 	= $(BUILD:.cpp=.o)
EXEC	= $(OBJS:.o=) 

.PHONY : all clean clan-all depend 

all: $(EXEC) forward_euler_octave
	@echo Compilation completed. Enjoy!

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $@.o -o $@

forward_euler_octave: forward_euler_octave.cc
	CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" $(MKOCTFILE) --link-stand-alone forward_euler_octave.cc -o forward_euler_octave

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	for dustfile in $(OBJS) $(EXEC) forward_euler_octave forward_euler_octave.o; do \
	!( test -e $$dustfile ) || $(RM) $$dustfile; \
	done

clean-all: clean

