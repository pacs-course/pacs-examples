CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -fopenmp -O3 -Wall -pedantic

LDFLAGS ?=
LIBS    ?=

EXEC = 01-hello_world_simple 01-hello_world_advanced 02-data_race 03-pi 03-pi_reduction

.PHONY = all $(EXEC) clean distclean

all: $(EXEC)

$(EXEC): %: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
	$(RM) *.csv *.out *.bak *~
