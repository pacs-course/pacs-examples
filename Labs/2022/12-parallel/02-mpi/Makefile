CXX       = mpic++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -Wall -pedantic

LDFLAGS ?=
LIBS    ?=

EXEC = 01-hello_world 02-ping_pong 03-probe 04-deadlock 04-non_blocking

.PHONY = all $(EXEC) clean distclean

all: $(EXEC)

$(EXEC): %: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
	$(RM) *.csv *.out *.bak *~
