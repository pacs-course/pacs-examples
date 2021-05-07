CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -Wall

EXEC     = 01-static 02-loop-unrolling 03-data-alignment
LDFLAGS ?=
LIBS    ?=

all: $(EXEC)

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXEC): %: %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC) $(EXECMP)
	$(RM) *~
