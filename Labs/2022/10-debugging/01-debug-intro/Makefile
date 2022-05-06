CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -Wall

EXEC     = assert1 assert2 debug_macro defensive \
	   pause print student1 student2 \
           undefined_behaviour undefined_behaviour2

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
	$(RM) $(EXEC)
	$(RM) *~
