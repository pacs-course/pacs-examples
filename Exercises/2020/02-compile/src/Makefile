CPPFLAGS=-I../include -I$(mkEigenInc) -I$(mkSuitesparseInc)
LDFLAGS=-L$(mkSuitesparseLib) -L. -Wl,-rpath=$(PWD)
LIBS=-lumfpack -llinearalgebra

all: liblinearalgebra.so test

test.o: test.cpp
	g++ $(CPPFLAGS) -c test.cpp

test: test.o
	g++ $(LDFLAGS) $(LIBS) test.o -o test

Utilities.o: Utilities.cpp
	g++ $(CPPFLAGS) -c -fPIC Utilities.cpp

liblinearalgebra.so: Utilities.o
	g++ -shared -Wl,-soname,liblinearalgebra.so Utilities.o -o liblinearalgebra.so

clean:
	rm *.o *.so test
