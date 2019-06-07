CXX ?= g++


all: integer-list

integer-list.o: integer-list.hpp integer-list.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c integer-list.cpp

integer-list: integer-list.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) integer-list.o -o integer-list

.PHONY: clean distclean

clean:
	$(RM) integer-list.o *~

distclean: clean
	$(RM) integer-list
