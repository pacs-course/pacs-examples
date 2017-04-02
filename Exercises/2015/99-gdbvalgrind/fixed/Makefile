CXX ?= g++
CXXFLAGS = --coverage

all: integer-list

integer-list.o: integer-list.h integer-list.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c integer-list.cpp

integer-list: integer-list.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) integer-list.o -o integer-list


.PHONY: clean distclean coverage

coverage: integer-list
	./integer-list
	lcov -c -o lcov-out.log --directory . -b . --no-external
	genhtml -o lcov-out lcov-out.log

clean:
	$(RM) integer-list.o *~ *.gcda *.gcno

distclean: clean
	$(RM) integer-list
	$(RM) -r lcov-out lcov-out.log

