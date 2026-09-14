# GCC 14 supports the explicit object parameters used by main_visit23.cpp.
# Override with, for example: make CXX=clang++-20

ifneq ($(filter default undefined,$(origin CXX)),)
CXX := g++-14 #use a compier supporting deducing this.
endif

CPPFLAGS ?=
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++23 
LDFLAGS ?=
LDLIBS ?=

PROGRAMS := main_visit_classic main_visit23

.PHONY: all run clean
.DELETE_ON_ERROR:

all: $(PROGRAMS)

$(PROGRAMS): %: %.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

run: all
	./main_visit_classic
	./main_visit23

clean:
	$(RM) $(PROGRAMS)
