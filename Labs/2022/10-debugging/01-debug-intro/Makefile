CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O0 -Wall -pedantic

LDFLAGS ?=
LIBS    ?=

EXEC = assert1 assert2 debug_macro defensive \
       pause print student1 student2 \
       undefined_behaviour undefined_behaviour2

.PHONY = all $(EXEC) clean distclean

all: $(EXEC)

$(EXEC): %: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
	$(RM) *~
