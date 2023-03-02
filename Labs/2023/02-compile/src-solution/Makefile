CXXFLAGS = -std=c++17 -O2 -fPIC
CPPFLAGS = -DNDEBUG -I${mkSuitesparseInc} -I${mkEigenInc} -I../include
LDFLAGS = -L. -L${mkSuitesparseLib} -Wl,-rpath=${PWD}
LIBS = -lumfpack -llinearalgebra

.PHONY: all clean distclean

all: test

test: test.o liblinearalgebra.so
	$(CXX) $(LDFLAGS) test.o -o test $(LIBS)

test.o: test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c test.cpp

liblinearalgebra.so: Utilities.o
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,liblinearalgebra.so \
	Utilities.o -o liblinearalgebra.so

Utilities.o: Utilities.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Utilities.cpp

clean:
	$(RM) *.o 

distclean: clean
	$(RM) liblinearalgebra.so test
